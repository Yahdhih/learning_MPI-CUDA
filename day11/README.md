# Day 11 — Cartesian Topologies · The Halo-Exchange Idea

## 🎯 Objectives
- Understand **domain decomposition** — how real simulations parallelize.
- Understand **ghost cells / halos** and why they exist.
- Use `MPI_Cart_create` / `MPI_Cart_shift` to find neighbors cleanly.

## 📖 Theory (25 min — heavier today, the exercise is short)

### Domain decomposition
A physical simulation (heat, fluid, weather) lives on a grid of points.
Updating each point needs its **neighbors** (a *stencil*):

```
new[i] = old[i] + α (old[i-1] − 2·old[i] + old[i+1])     (1D heat equation)
```
To parallelize: **cut the grid into chunks, one per rank**. Rank r updating
its leftmost point needs `old[i-1]`… which lives on rank r−1. This is where
messages come in.

### Ghost cells (halos)
Instead of asking the neighbor for each point on demand (way too many tiny
messages), each rank stores **one extra cell on each side** — copies of the
neighbor's edge values:

```
rank r's array:   [ghostL | own cell 1 … own cell n | ghostR]
                      ↑                                  ↑
              copy of r−1's last cell        copy of r+1's first cell
```
Each iteration:
1. **Halo exchange**: swap edge values with both neighbors
   (a perfect job for `MPI_Sendrecv` — day 03!),
2. update all own cells using purely local reads.

One message pair per neighbor per step, regardless of chunk size. This
pattern powers most of the world's HPC codes.

### Cartesian topology helpers
You could compute neighbors by hand (`prev = rank-1`, careful at borders).
MPI does it for you, plus it may map ranks to hardware more efficiently:

```c
int dims[1]    = {size};
int periodic[1] = {0};          /* 0: borders have no neighbor           */
MPI_Comm cart;
MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periodic, 1, &cart);

int left, right;
MPI_Cart_shift(cart, 0 /*dim*/, 1 /*step*/, &left, &right);
/* at the borders, left/right = MPI_PROC_NULL */
```
The magic of `MPI_PROC_NULL`: sending to / receiving from it is a **legal
no-op**. So border ranks run the *same* Sendrecv code as interior ranks —
no `if` at the edges!

## 💻 Practice (30 min)

1. Run `neighbors.c`: each rank prints its left/right neighbor from
   `MPI_Cart_shift` (−1 shown at borders). Try periodic = 1 too (edit,
   rebuild): the line becomes a ring.
2. **Exercise** — `exercises/ex1_halo.c`: one halo exchange, no physics yet.
   Each rank owns 4 cells `[rank*4 … rank*4+3]` plus 2 ghosts. Complete:
   - the two `MPI_Sendrecv` calls (leftward and rightward),
   - using `MPI_PROC_NULL` neighbors from `MPI_Cart_shift` — no border ifs.
   Verify each rank prints ghosts equal to its neighbors' edge values.

Tomorrow this exchange becomes the inner loop of a real heat simulation.

## ✅ Self-check (5 min)
1. Why halos instead of requesting neighbor values point by point?
2. What does Sendrecv with source `MPI_PROC_NULL` put in the buffer?
3. For a 2D grid split into P×Q tiles, how many halo exchanges per step?

<details><summary>Answers</summary>

1. Message *latency* dominates small messages; batching all edge values
   into one message per neighbor per step amortizes it.
2. Nothing — the buffer is untouched; the call returns immediately.
3. Four (up/down/left/right) — usually two Sendrecvs per dimension.
</details>
