/* Day 03 — Exercise 1 solution (version B active, version A shown). */
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

#if 0   /* --- version A: break the symmetry ------------------------------ */
    if (rank == 0) {
        MPI_Send(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD);
        MPI_Recv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD);
    }
#else   /* --- version B: MPI_Sendrecv ------------------------------------ */
    MPI_Sendrecv(mine,   N, MPI_INT, other, 0,
                 theirs, N, MPI_INT, other, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif

    printf("rank %d: got array full of %d — no deadlock!\n",
           rank, theirs[0]);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
