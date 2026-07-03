/* Day 06 — Exercise 2 solution. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int my_val = rank * rank;
    int *vals = malloc(size * sizeof(int));

    MPI_Allgather(&my_val, 1, MPI_INT, vals, 1, MPI_INT, MPI_COMM_WORLD);

    printf("rank %d sees:", rank);
    for (int i = 0; i < size; i++) printf(" %d", vals[i]);
    printf("\n");

    free(vals);
    MPI_Finalize();
    return 0;
}
