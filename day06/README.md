# Day 06 — Collectives II: `MPI_Scatter` / `MPI_Gather` / `MPI_Allgather`

## 🎯 Objectives
- Distribute an array across ranks with `Scatter`, collect with `Gather`.
- Understand the `sendcount` trap (it's *per rank*, not total).
- Know when to use `Allgather` vs `Gather`.

## 📖 Theory (20 min)

### The data-distribution pattern
Most parallel programs look like:
1. Root has (or creates) a big array.
2. **Scatter**: each rank gets one equal chunk.
3. Each rank computes on its chunk (this is the parallel speedup).
4. **Gather**: root collects the results.

### `MPI_Scatter`
```c
double big[N];            /* meaningful only on root      */
double chunk[N_PER];      /* N_PER = N / size, every rank */

MPI_Scatter(big,   N_PER, MPI_DOUBLE,    /* send: from root  */
            chunk, N_PER, MPI_DOUBLE,    /* recv: every rank */
            0, MPI_COMM_WORLD);
```
⚠️ **Trap #1**: `sendcount` = number of elements **sent to EACH rank**, not
the total. `Scatter(big, N, ...)` when you meant `N/size` = classic bug.

⚠️ **Trap #2**: rank *i* gets elements `[i*N_PER, (i+1)*N_PER)` — the chunks
are assigned in rank order, deterministically.

Root also receives its own chunk (chunk 0). The `big` argument is ignored on
non-root ranks (pass `NULL` there if you like).

### `MPI_Gather` — the exact inverse
```c
MPI_Gather(chunk, N_PER, MPI_DOUBLE,     /* everyone sends its chunk */
           big,   N_PER, MPI_DOUBLE,     /* root receives all, in rank order */
           0, MPI_COMM_WORLD);
```
Again: `recvcount` = elements received **from each rank**.

### `MPI_Allgather` — gather + everyone gets the result
Same as Gather but the full array ends up **on every rank** (no root
argument). Use when every rank needs everyone's results to continue.
`Allgather ≈ Gather + Bcast`, but faster as one fused operation.

(If chunks have different sizes: `MPI_Scatterv` / `MPI_Gatherv` — just know
they exist.)

## 💻 Practice (35 min)

1. Run `scatter_demo.c` and match every printed number to the theory.
2. **Exercise 1** — `exercises/ex1_avg.c`: parallel average.
   - Root fills `data[N]` with `data[i] = i` (N = 16'000'000).
   - Scatter; each rank computes the **sum** of its chunk;
   - Gather the partial sums (1 double per rank) to root;
   - Root adds them and prints the average (expected: (N-1)/2 = 7999999.5).
3. **Exercise 2** — `exercises/ex2_allgather.c`: each rank computes
   `my_val = rank*rank`, then everyone builds the full table
   `vals[size]` with **one** `MPI_Allgather` and prints it.

## ✅ Self-check (5 min)
1. `MPI_Scatter(big, 100, MPI_INT, chunk, 100, MPI_INT, 0, comm)` with 4
   ranks: how many ints does rank 2 receive, and which ones?
2. Why does Gather of the partial sums use count = 1?
3. When is Allgather the wrong choice (hint: memory)?

<details><summary>Answers</summary>

1. 100 ints: elements 200..299. (Count is per rank!)
2. Each rank contributes a single double (its partial sum).
3. When the gathered array is huge and only one rank actually needs it —
   Allgather replicates it in every process's memory.
</details>
