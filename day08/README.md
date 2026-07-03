# Day 08 — Mini-Project: Parallel Numerical Integration

## 🎯 Objectives
- Consolidate week 1 in one program written mostly from scratch.
- Use **block distribution** (vs day 07's cyclic) and defend the choice.
- Structure a real MPI program: input → broadcast → compute → reduce.

## 📖 Theory (15 min — shorter today, more coding)

### The trapezoidal rule
Integral of f on [a,b] with n trapezoids of width h = (b-a)/n:

```
∫ ≈ h · [ f(a)/2 + f(x₁) + f(x₂) + … + f(x_{n-1}) + f(b)/2 ]
```

### Block distribution
Day 07 used cyclic (i += size). Today each rank takes one **contiguous
block** of trapezoids:

```c
long per   = n / size;
long rem   = n % size;                 /* leftover when size ∤ n        */
long lo    = rank * per + (rank < rem ? rank : rem);
long count = per + (rank < rem ? 1 : 0);
/* rank handles trapezoids [lo, lo+count) */
```
This formula distributes the remainder over the first `rem` ranks — learn
it, you'll reuse it constantly. For integration, cyclic and block give the
same math; block matters when memory locality matters (arrays — day 12).

### Program shape (the standard MPI skeleton)
```
1. MPI_Init, rank/size
2. rank 0 parses argv (a, b, n)          ← input on root only
3. Bcast a, b, n                          ← everyone gets parameters
4. compute my [lo, lo+count) partial sum  ← the parallel part
5. Reduce(MPI_SUM) to root                ← combine
6. root prints result, error, time        ← output on root only
7. MPI_Finalize
```

## 💻 Project (40 min)

Complete `exercises/trap.c`. It integrates f(x) = sin(x) on [0, π]
(exact answer: **2**).

```sh
make
mpirun -np 4 ./trap 0 3.14159265358979 10000000
```

Requirements:
1. Only rank 0 reads `argv`; broadcast `a`, `b` (doubles) and `n` (long —
   use `MPI_LONG`).
2. Block distribution with the remainder formula (test with `-np 3` and
   n = 10 → blocks of 4, 3, 3).
3. Reduce the partial sums; root prints result, |error|, and `MPI_Wtime`
   elapsed.
4. Measure and note speedup for np = 1, 2, 4 with n = 10⁸.

### Stretch goals (if time remains — else skip, day 09 doesn't need them)
- Add `-DF_SQUARE` support: integrate x² on [0,1] (exact: 1/3).
- Print each rank's `lo`/`count` and check the remainder distribution.

## ✅ Self-check (5 min)
1. Why does each *interior* point count once but endpoints half?
2. With n = 10, size = 3: which trapezoids does each rank own?
3. Where would this program spend its time if n were tiny (say 100) and
   np huge? What lesson does that teach?

<details><summary>Answers</summary>

1. Each interior x is shared by two adjacent trapezoids (right edge of one,
   left of next) → its f(x) appears twice at weight ½ = once.
2. rank 0: [0,4), rank 1: [4,7), rank 2: [7,10).
3. In MPI overhead (init + collectives), not math. Parallelism only pays
   when there's enough work per process — Amdahl's law in action.
</details>
