# Day 09 — Derived Datatypes: `MPI_Type_vector` & Friends

## 🎯 Objectives
- Send non-contiguous data (a matrix column!) in **one** call.
- Learn the create → commit → use → free lifecycle.
- Send a C struct with `MPI_Type_create_struct`.

## 📖 Theory (20 min)

### The problem
`MPI_Send(buf, count, MPI_DOUBLE, …)` sends `count` doubles sitting
**contiguously** in memory. But real data often isn't contiguous:
- a **column** of a C matrix (C stores row-major: rows are contiguous,
  columns are strided),
- a struct mixing ints and doubles,
- every other element of an array.

Copying into a temp buffer works but is ugly and slow. MPI's answer:
describe the layout once as a **derived datatype**, then use it like any
other datatype.

### `MPI_Type_vector` — strided data
```c
/* a column of an N×N row-major matrix double m[N][N]:
   N blocks of 1 double, stride N doubles apart */
MPI_Datatype coltype;
MPI_Type_vector(N,        /* count  : number of blocks       */
                1,        /* blocklen: elements per block    */
                N,        /* stride : elements between block STARTS */
                MPI_DOUBLE, &coltype);
MPI_Type_commit(&coltype);              /* must commit before use */

MPI_Send(&m[0][2], 1, coltype, dest, 0, MPI_COMM_WORLD);  /* column 2! */
...
MPI_Type_free(&coltype);                /* when done for good */
```
Note `&m[0][2]`: the datatype describes offsets *relative to the start
address you pass*.

The receiver may receive it as a plain contiguous `N × MPI_DOUBLE` buffer —
**types describe layout, not identity**; only the total type signature
(sequence of basic elements) must match.

### `MPI_Type_create_struct` — heterogeneous data
```c
typedef struct { int id; double x, y; } Particle;

int          lens[2]  = {1, 2};
MPI_Aint     disps[2] = {offsetof(Particle, id), offsetof(Particle, x)};
MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};

MPI_Datatype ptype;
MPI_Type_create_struct(2, lens, disps, types, &ptype);
MPI_Type_commit(&ptype);
/* now: MPI_Send(&p, 1, ptype, ...) or even arrays of Particle */
```
(`offsetof` from `<stddef.h>` handles compiler padding correctly.)

Also exists: `MPI_Type_contiguous` (n of a type glued together).

## 💻 Practice (35 min)

1. Run `column_demo.c`: rank 0 sends column 2 of a 6×6 matrix to rank 1
   with a vector type; rank 1 receives it as a flat array and prints it.
2. **Exercise 1** — `exercises/ex1_column_swap.c`: complete the TODOs so
   rank 0 sends column 0 and rank 1 replies with **its row 0** — read the
   asymmetry carefully: a row is already contiguous, so only one side
   needs a vector type!
3. **Exercise 2** — `exercises/ex2_particle.c`: build the `Particle`
   struct type; rank 0 sends an array of 5 particles to rank 1, which
   prints them and verifies the values.

## ✅ Self-check (5 min)
1. In `MPI_Type_vector(N, 1, N, MPI_DOUBLE, …)`, why is the stride N?
2. What happens if you forget `MPI_Type_commit`?
3. Why use `offsetof` instead of assuming `disps = {0, 4}`?

<details><summary>Answers</summary>

1. Consecutive elements of a column are one full row apart in row-major
   memory, i.e. N doubles.
2. Using an uncommitted type in communication is an error — commit
   finalizes the internal representation.
3. The compiler may insert padding between `id` and `x` (here typically 4
   bytes, so x is at offset 8, not 4). `offsetof` is always right.
</details>
