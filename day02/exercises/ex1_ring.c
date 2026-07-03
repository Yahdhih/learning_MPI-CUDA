/* Day 02 — Exercise 1: token ring.
 *
 * Rank 0 starts with token = 0, sends to rank 1.
 * Each rank: receives from (rank-1), adds its rank, prints, sends to
 * (rank+1) % size. Rank 0 receives the final token from rank size-1.
 *
 * With -np 4 the final token must be 0+1+2+3 = 6.
 *
 * Build:  make
 * Run:    mpirun -np 4 ./ex1_ring
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int token;
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    if (rank == 0) {
        token = 0;
        /* TODO 1: send token to `next` (rank 1) */

        /* TODO 2: receive the final token from `prev` (last rank)
         *         and print "final token = %d" */
    } else {
        /* TODO 3: receive token from `prev` */

        /* TODO 4: add my rank to token, print
         *         "rank %d: token is now %d" */

        /* TODO 5: send token to `next` */
    }

    MPI_Finalize();
    return 0;
}
