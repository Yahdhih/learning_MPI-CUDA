# Day 01 — Parallel Computing Intro · MPI Hello World · Ranks & Size

## 🎯 Objectives
- Understand *why* parallel computing exists and the two main models.
- Compile and run your first MPI program.
- Understand **rank**, **size**, and the SPMD execution model.

## 📖 Theory (20 min)

### Why parallel computing?
Single-core speed stopped improving around 2005 (power/heat limits).
Performance now comes from doing many things at once:
- **Shared memory** (threads, OpenMP): several cores, *one* memory. Easy but
  limited to one machine.
- **Distributed memory** (MPI): many independent processes, each with its
  *own* memory, cooperating by **sending messages**. Scales to thousands of
  machines — this is how supercomputers work.
- **GPU** (CUDA — phase 2): thousands of tiny cores doing the same operation
  on different data.

### What is MPI?
MPI = *Message Passing Interface*. It is a **standard** (a specification of
~430 C functions), not a language. Implementations: Open MPI (what you have),
MPICH, Intel MPI. You write ordinary C, include `mpi.h`, compile with the
wrapper `mpicc`, and launch with `mpirun`.

### The SPMD model — the single most important idea
**S**ingle **P**rogram, **M**ultiple **D**ata:

```sh
mpirun -np 4 ./program
```
launches **4 identical copies** of your program as 4 independent OS processes.
They do NOT share variables. Each copy asks MPI:
- *"How many of us are there?"* → `MPI_Comm_size` → **size** (here 4)
- *"Which one am I?"* → `MPI_Comm_rank` → **rank** (0, 1, 2 or 3)

All different behavior comes from branching on the rank:
`if (rank == 0) { ... } else { ... }`.

### The minimal skeleton (memorize this)
```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);                    // start MPI (always first)

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // my id: 0..size-1
    MPI_Comm_size(MPI_COMM_WORLD, &size);      // total number of processes

    printf("Hello from rank %d of %d\n", rank, size);

    MPI_Finalize();                            // stop MPI (always last)
    return 0;
}
```
`MPI_COMM_WORLD` is the default **communicator**: the group containing all
launched processes. Almost every MPI call takes a communicator argument.

## 💻 Practice (35 min)

1. Build and run the provided hello world:
   ```sh
   cd day01
   make
   mpirun -np 4 ./hello
   ```
   Run it several times — **notice the output order changes**. Processes are
   independent; nothing orders their prints.
2. Try `-np 1`, `-np 2`, `-np 8` (add `--oversubscribe` if 8 > your cores).
3. Open `exercises/ex1_roles.c` and complete the TODOs:
   - rank 0 prints `"I am the coordinator of N processes"`,
   - every other rank prints `"I am worker <rank>"`,
   - **every even rank** additionally prints `"my rank is even"`.
4. Build with `make` and check with `mpirun -np 5 ./ex1_roles`.

## ✅ Self-check (5 min)
1. If you run `mpirun -np 4 ./hello`, how many times does `main()` execute?
2. Do two ranks share the same variable `rank`? Why not?
3. Why is the print order non-deterministic?

<details><summary>Answers</summary>

1. 4 times — 4 separate processes each run the whole program.
2. No. Each process has its own address space; they are separate programs
   from the OS point of view.
3. The OS schedules the processes independently; MPI imposes no order unless
   you add communication.
</details>
