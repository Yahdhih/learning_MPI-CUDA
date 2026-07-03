# Day 03 — Deadlocks · `MPI_Sendrecv` · Tags & Status

## 🎯 Objectives
- Recognize and fix the classic exchange deadlock.
- Use `MPI_Sendrecv` to exchange data safely.
- Use tags and `MPI_Status` to identify messages.

## 📖 Theory (20 min)

### The classic deadlock
Two processes want to **exchange** values:
```c
/* BOTH ranks execute this — looks symmetric and innocent: */
MPI_Send(&mine,   1, MPI_INT, other, 0, MPI_COMM_WORLD);
MPI_Recv(&theirs, 1, MPI_INT, other, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```
For **large** messages, `MPI_Send` waits until the other side posts a `Recv`.
Both ranks sit in `Send`, neither reaches `Recv` → both wait forever.
(For small messages MPI buffers internally and it *happens* to work — the
worst kind of bug: it appears only when data grows.)

### Fix 1 — break the symmetry
```c
if (rank == 0) { Send; Recv; }
else           { Recv; Send; }
```

### Fix 2 — `MPI_Sendrecv` (the pro way)
One call that sends and receives simultaneously; MPI guarantees no deadlock:
```c
MPI_Sendrecv(&mine,   1, MPI_INT, other, 0,      /* what I send, to whom  */
             &theirs, 1, MPI_INT, other, 0,      /* what I get, from whom */
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```
This is *the* tool for halo exchanges (day 11–13).

### Tags & status
- **Tag** = integer label. Use different tags to distinguish message kinds
  flowing between the same pair of processes.
- When you receive with wildcards (`MPI_ANY_SOURCE` / `MPI_ANY_TAG`), the
  `MPI_Status` struct tells you what actually arrived:
```c
MPI_Status st;
MPI_Recv(buf, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
printf("from %d, tag %d\n", st.MPI_SOURCE, st.MPI_TAG);
int n; MPI_Get_count(&st, MPI_INT, &n);   /* how many elements arrived */
```

## 💻 Practice (35 min)

1. Build & run `deadlock.c` with a LARGE message:
   ```sh
   make && mpirun -np 2 ./deadlock
   ```
   It hangs → `Ctrl-C`. Read the code and make sure you see why.
2. **Exercise 1** — `exercises/ex1_exchange.c`: fix the deadlock **twice**:
   - version A: break the symmetry with `if (rank == 0)`,
   - version B: use `MPI_Sendrecv` (comment version A out).
3. **Exercise 2** — `exercises/ex2_anysource.c`: every worker rank sends its
   rank×10 to rank 0 with tag = its rank. Rank 0 receives `size-1` messages
   with `MPI_ANY_SOURCE`/`MPI_ANY_TAG` and prints, for each, the source and
   tag from the status. Note the arrival order varies between runs.

## ✅ Self-check (5 min)
1. Why does the deadlock only appear with large messages?
2. In `MPI_Sendrecv`, can the send-buffer and recv-buffer be the same array?
3. When do you actually need to read `MPI_Status`?

<details><summary>Answers</summary>

1. Small sends are copied into an internal MPI buffer and return
   immediately ("eager" protocol); large sends wait for the receiver
   ("rendezvous" protocol).
2. No — use `MPI_Sendrecv_replace` for that, or separate buffers.
3. Only when you received with a wildcard (unknown source/tag) or need the
   actual received count via `MPI_Get_count`.
</details>
