/* Day 01 — Exercise 1: roles by rank.
 *
 * Goal:
 *   - rank 0 prints:            "I am the coordinator of <size> processes"
 *   - every other rank prints:  "I am worker <rank>"
 *   - every EVEN rank ALSO prints: "my rank is even"
 *
 * Build:  make
 * Run:    mpirun -np 5 ./ex1_roles
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    /* TODO 1: initialize MPI */

    int rank, size;
    /* TODO 2: get rank and size from MPI_COMM_WORLD */

    /* TODO 3: branch on rank to print the right message */

    /* TODO 4: even ranks print the extra line */

    /* TODO 5: finalize MPI */
    return 0;
}
