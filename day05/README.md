# Day 05 — Collectives I: `MPI_Bcast` · `MPI_Barrier`

## 🎯 Objectives
- Understand what a **collective** operation is and its one big rule.
- Broadcast data from one rank to all with `MPI_Bcast`.
- Know what `MPI_Barrier` does — and when *not* to use it.

## 📖 Theory (20 min)

### Collectives
A collective is an operation that **every rank in the communicator must
call**. The library implements them with clever algorithms (trees,
pipelines) that beat anything you'd hand-write with Send/Recv.

**The one big rule:** *all* ranks must reach the call — same operation, same
communicator, compatible arguments. If one rank skips it (e.g. it's inside
an `if (rank == 0)`), everyone else hangs. This is the #1 collective bug.

### `MPI_Bcast` — one to all
```c
int params[3];
if (rank == 0) { /* only root fills the data */
    params[0] = 100; params[1] = 42; params[2] = 7;
}
MPI_Bcast(params, 3, MPI_INT, 0 /* root */, MPI_COMM_WORLD);
/* now EVERY rank has the same params[] */
```
Note the asymmetric reading: on the root, `params` is an **input**; on all
other ranks it's an **output**. But *everyone calls the same line*.

Typical use: rank 0 reads a config file / user input, broadcasts it.

Internally Bcast is a **tree**: root sends to 1, then 0 and 1 each send to
one more, … → `log₂(P)` steps instead of `P-1`. That's why you never write
a for-loop of Sends to distribute the same data.

### `MPI_Barrier` — everyone waits for everyone
```c
MPI_Barrier(MPI_COMM_WORLD);
```
No rank leaves the barrier until all ranks have entered it.

**When you need it:** almost only around timing measurements (start the
chronometer when everyone is ready) and debugging.
**When you don't:** to "order" communication — Send/Recv already
synchronize the pair involved. Sprinkling barriers to fix bugs usually
hides races instead of fixing them, and costs performance.

## 💻 Practice (35 min)

1. Run `bcast_demo.c` — rank 0 "reads" a simulation config and broadcasts it.
2. **Exercise 1** — `exercises/ex1_settings.c`:
   - rank 0 initializes `n = 1000` and `alpha = 0.25`,
   - broadcast both to all ranks (two Bcasts, or be clever with a struct later),
   - every rank prints them, then computes and prints its own slice bounds
     `[rank*n/size, (rank+1)*n/size)` — the standard way to split work.
3. **Exercise 2** — `exercises/ex2_guess.c` (fun one): rank 0 picks a
   "secret" number (from `argv`), broadcasts it; each rank checks whether
   its rank matches the secret and the matching rank prints "I am the
   chosen one". Silly, but it forces you to respect the big rule.
4. Optional experiment: put the Bcast inside `if (rank == 0)` and watch the
   hang. Understand *why* (compare with the big rule). Ctrl-C.

## ✅ Self-check (5 min)
1. Why is Bcast to P ranks O(log P) and not O(P)?
2. What happens if only rank 0 calls `MPI_Bcast`?
3. Does `MPI_Barrier` guarantee that all prints before it appear before
   prints after it?

<details><summary>Answers</summary>

1. Tree algorithm: each round doubles the number of ranks holding the data.
2. Rank 0 may block (or "succeed" and continue), every other rank blocks
   forever when they reach their *next* collective — hang or corruption.
3. On screen, mostly yes in practice but stdout merging has no hard
   guarantee; the barrier orders the *processes*, not the terminal buffering.
</details>
