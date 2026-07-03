/* Day 03 — Exercise 1: fix the deadlock two ways.
 *
 * Two ranks must exchange their arrays. The naive Send-then-Recv deadlocks
 * for large N. Fix it:
 *   version A: break the symmetry (rank 0: Send,Recv — rank 1: Recv,Send)
 *   version B: a single MPI_Sendrecv call
 *
 * Run: mpirun -np 2 ./ex1_exchange
 * Success = the final printf actually appears.
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

    /* --- version A: TODO — break the symmetry with an if/else on rank --- */



    /* --- version B: TODO — replace version A with one MPI_Sendrecv ------ */



    printf("rank %d: got array full of %d — no deadlock!\n",
           rank, theirs[0]);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
