/* Day 01 — Exercise 1 solution. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
        printf("I am the coordinator of %d processes\n", size);
    else
        printf("I am worker %d\n", rank);

    if (rank % 2 == 0)
        printf("my rank is even\n");

    MPI_Finalize();
    return 0;
}
