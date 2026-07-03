/* Day 03 — a REAL deadlock, provided on purpose. Run with -np 2 and watch
 * it hang (kill with Ctrl-C). Both ranks block in MPI_Send because the
 * message is too large to be buffered internally.
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1 << 22)   /* 4M ints = 16 MB : forces rendezvous protocol */

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int other = 1 - rank;

    int *mine   = malloc(N * sizeof(int));
    int *theirs = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) mine[i] = rank;

    printf("rank %d: sending...\n", rank);
    MPI_Send(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD);   /* blocks! */
    MPI_Recv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("rank %d: done (you will never see this line)\n", rank);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
