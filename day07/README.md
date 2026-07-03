# Day 07 — `MPI_Reduce` / `MPI_Allreduce` · Computing π in Parallel

## 🎯 Objectives
- Combine values from all ranks with one call: `MPI_Reduce`.
- Know the built-in operations (`MPI_SUM`, `MPI_MAX`, …).
- Write the classic parallel π program — your first *real* speedup.

## 📖 Theory (20 min)

### `MPI_Reduce` — combine all ranks' values
Yesterday's ex1 (gather partial sums, then loop to add them) is such a
common pattern that MPI fuses it into one optimized call:

```c
double partial = ...;   /* each rank's local result   */
double total;           /* meaningful on root only    */
MPI_Reduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
```
Internally it's a tree again: O(log P) instead of O(P).

Built-in ops: `MPI_SUM`, `MPI_PROD`, `MPI_MAX`, `MPI_MIN`, `MPI_LAND`,
`MPI_LOR`, `MPI_MAXLOC`/`MPI_MINLOC` (value *and* who had it).

With `count > 1` the reduction is **element-wise**: reducing arrays of 3
doubles gives an array of 3 sums.

### `MPI_Allreduce` — everyone gets the result
```c
MPI_Allreduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
```
No root: every rank ends up with `total`. This is *the most used collective
in real HPC codes* (residual norms in iterative solvers, dot products, …).

### The π algorithm (numerical integration)
π = ∫₀¹ 4/(1+x²) dx. Approximate with the midpoint rule over `n` slices:

```
h = 1/n
π ≈ h · Σ_{i=0}^{n-1}  4 / (1 + x_i²),   x_i = h·(i + 0.5)
```
Parallelization: **cyclic distribution** — rank r handles i = r, r+size,
r+2·size, … Every rank computes an independent partial sum; one `Reduce`
finishes the job. No input data to distribute at all!

## 💻 Practice (35 min)

1. **Exercise 1** — `exercises/ex1_pi.c` (the main course):
   complete the TODOs, then verify convergence:
   ```sh
   mpirun -np 4 ./ex1_pi 100000000
   ```
   Expected: error ~1e-15, and the time printed should shrink as you go
   from `-np 1` to `-np 4` (that's your speedup!).
2. **Exercise 2** — `exercises/ex2_minmax.c`: each rank draws a random int
   (seeded by rank); use **two reduces** (`MPI_MIN`, `MPI_MAX`) so rank 0
   can print the global min and max. Bonus: use `MPI_MINLOC` on a
   `{value, rank}` pair (type `MPI_2INT`) to also learn *which* rank had it.
3. Record in a note: times for `-np 1,2,4` and computed speedup t₁/tₚ.

## ✅ Self-check (5 min)
1. Reduce vs Allreduce — when do you need which?
2. Why does the π program scale almost perfectly (near-linear speedup)?
3. Reduce with `count = 3` on arrays `{1,2,3}` and `{10,20,30}` gives?

<details><summary>Answers</summary>

1. Reduce: only root needs the result (e.g. final answer to print).
   Allreduce: everyone needs it to continue (e.g. convergence check).
2. Zero data distribution, one tiny communication at the end; the work is
   embarrassingly parallel.
3. `{11,22,33}` — element-wise.
</details>
