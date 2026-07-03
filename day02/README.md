# Day 02 — Point-to-Point: `MPI_Send` / `MPI_Recv`

## 🎯 Objectives
- Send data from one process to another.
- Understand the 6 arguments of `MPI_Send` and the 7 of `MPI_Recv`.
- Understand that `MPI_Recv` **blocks** until a matching message arrives.

## 📖 Theory (20 min)

### The two fundamental calls
Everything in MPI can be built from these two:

```c
MPI_Send(&data, count, MPI_INT, dest,   tag, MPI_COMM_WORLD);
MPI_Recv(&data, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
```

| Argument | Meaning |
|----------|---------|
| `&data`  | pointer to the buffer to send / where to store |
| `count`  | number of **elements** (not bytes!) |
| `MPI_INT`| datatype: `MPI_INT`, `MPI_DOUBLE`, `MPI_CHAR`, `MPI_FLOAT`, … |
| `dest` / `source` | rank of the other process |
| `tag`    | an integer label you choose (0 is fine); Send tag must match Recv tag |
| comm     | the communicator, `MPI_COMM_WORLD` for now |
| `&status`| (Recv only) info about the received message; `MPI_STATUS_IGNORE` if unused |

### Matching
A `Recv` matches a `Send` when **(source, tag, communicator)** agree.
`Recv` may use wildcards: `MPI_ANY_SOURCE`, `MPI_ANY_TAG`.

### Blocking semantics — the #1 source of bugs
- `MPI_Recv` **always blocks**: the process stops until the message is there.
- `MPI_Send` blocks until the buffer is safe to reuse. For *small* messages
  the library usually copies to an internal buffer and returns immediately;
  for *large* ones it waits for the receiver. **Never rely on the buffered
  behavior** — assume `Send` may wait for a matching `Recv`.

If a `Recv` has no matching `Send` (or vice-versa for large messages), the
program hangs forever. That's a **deadlock** — day 03's topic.
Kill a hung run with `Ctrl-C`.

### The classic pattern
```c
if (rank == 0) {
    int x = 42;
    MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
} else if (rank == 1) {
    int x;
    MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("rank 1 received %d\n", x);
}
```
Note: with `-np 4`, ranks 2 and 3 simply do nothing here. That's fine.

## 💻 Practice (35 min)

1. Read, build, run the provided `pingpong.c` (`make && mpirun -np 2 ./pingpong`).
   Rank 0 sends a counter to rank 1, rank 1 increments and sends it back, 10 times.
2. **Exercise 1** — `exercises/ex1_ring.c`: pass a token around a ring.
   - Rank 0 starts with `token = 0` and sends it to rank 1.
   - Each rank receives from `rank-1`, **adds its own rank** to the token,
     prints it, and sends it to `(rank+1) % size`.
   - Rank 0 finally receives it back from the last rank and prints
     `"final token = X"`.
   - Check: for `-np 4`, final token = 0+1+2+3 = **6**.
3. Break it on purpose: make rank 0 `Recv` *before* it `Send`s. Observe the
   hang. `Ctrl-C`, then put it back.

## ✅ Self-check (5 min)
1. `count` is in elements or bytes?
2. What three things must match for a Recv to accept a Send?
3. Why must rank 0 in the ring send *before* receiving, while all other
   ranks receive *before* sending?

<details><summary>Answers</summary>

1. Elements. `MPI_Send(buf, 10, MPI_INT, ...)` sends 10 ints (40 bytes).
2. Source rank, tag, communicator (and the receive buffer must be big enough).
3. Someone must inject the first message. If everyone receives first, every
   process waits and nobody ever sends: deadlock.
</details>
