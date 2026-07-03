/* Day 04 — Exercise 1 solution. */
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
    MPI_Irecv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[1]);
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);

    printf("rank %d: got array full of %d\n", rank, theirs[0]);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
