# Day 04 — Non-blocking: `MPI_Isend` / `MPI_Irecv` / `MPI_Wait`

## 🎯 Objectives
- Start communication without waiting for it (`I` = *immediate*).
- Use `MPI_Request` and `MPI_Wait` / `MPI_Waitall` correctly.
- Know the golden rule: **don't touch the buffer until the Wait**.

## 📖 Theory (20 min)

### Why non-blocking?
1. **Deadlock-freedom**: `Isend`/`Irecv` return immediately, so the
   symmetric-exchange deadlock of day 03 cannot happen.
2. **Overlap**: start a communication, do useful computation while the
   network works, then wait. This hides communication cost — essential at
   scale.

### The API
```c
MPI_Request req;
MPI_Isend(&x, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &req);  /* starts it  */
/* ... do other work — but do NOT modify x ... */
MPI_Wait(&req, MPI_STATUS_IGNORE);                            /* finishes it */
```
Same for `MPI_Irecv` (do not *read* the buffer before the Wait).

Several at once:
```c
MPI_Request reqs[2];
MPI_Irecv(..., &reqs[0]);
MPI_Isend(..., &reqs[1]);
MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
```

`MPI_Test(&req, &done, &status)` is the polling version: checks without
blocking.

### The golden rule
Between `Isend` and its `Wait`, the send buffer is **on loan to MPI**:
- after `Isend`: don't **write** to the buffer,
- after `Irecv`: don't **read or write** the buffer.

Every non-blocking call **must** eventually be matched by a `Wait`/`Test`
that completes it — otherwise you leak requests.

### Safe symmetric exchange (compare with day 03!)
```c
MPI_Request reqs[2];
MPI_Irecv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[0]);
MPI_Isend(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[1]);
MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
```
Post the `Irecv` **first** — it's free, and it gives MPI a place to put the
incoming data immediately.

## 💻 Practice (35 min)

1. Read & run `overlap.c`: it measures the same exchange done blocking vs.
   non-blocking-with-computation-overlap and prints both timings.
2. **Exercise 1** — `exercises/ex1_iexchange.c`: yesterday's exchange, but
   with `Irecv` + `Isend` + `Waitall`. Must not deadlock at N = 4M ints.
3. **Exercise 2** — `exercises/ex2_ring_nb.c`: the day-02 token ring, but
   every rank posts `Irecv` first, then `Isend`, then waits. Note that even
   rank 0 needs no special ordering anymore.

## ✅ Self-check (5 min)
1. What does `MPI_Isend` guarantee when it returns?
2. Why post `Irecv` before `Isend`?
3. Is it legal to `MPI_Wait` on a request twice?

<details><summary>Answers</summary>

1. Nothing about the message! Only that the operation has *started*; the
   buffer is not yet reusable.
2. So the receiver's buffer is already registered when the data arrives —
   avoids extra copies and can avoid rendezvous stalls.
3. Yes but useless: after the first Wait, the request becomes
   `MPI_REQUEST_NULL` and a second Wait returns immediately.
</details>
