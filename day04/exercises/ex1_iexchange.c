/* Day 04 — Exercise 1: symmetric exchange with non-blocking calls.
 *
 * Same task as day 03 ex1, but you must use Irecv + Isend + Waitall.
 * Run: mpirun -np 2 ./ex1_iexchange
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1 << 22)

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int other = 1 - rank;

    int *mine   = malloc(N * sizeof(int));
    int *theirs = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) mine[i] = rank;

    MPI_Request reqs[2];

    /* TODO 1: post the Irecv for `theirs` into reqs[0] */

    /* TODO 2: post the Isend of `mine`  into reqs[1] */

    /* TODO 3: wait for both with MPI_Waitall */

    printf("rank %d: got array full of %d\n", rank, theirs[0]);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
