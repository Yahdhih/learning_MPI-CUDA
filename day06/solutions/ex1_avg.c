/* Day 06 — Exercise 1 solution. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 16000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int per = N / size;

    double *data = NULL;
    if (rank == 0) {
        data = malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) data[i] = (double)i;
    }

    double *chunk = malloc(per * sizeof(double));
    MPI_Scatter(data, per, MPI_DOUBLE, chunk, per, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    double partial = 0.0;
    for (int i = 0; i < per; i++) partial += chunk[i];

    double *partials = NULL;
    if (rank == 0) partials = malloc(size * sizeof(double));

    MPI_Gather(&partial, 1, MPI_DOUBLE, partials, 1, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        double total = 0.0;
        for (int i = 0; i < size; i++) total += partials[i];
        printf("average = %f (expected %f)\n", total / N, (N - 1) / 2.0);
        free(partials); free(data);
    }

    free(chunk);
    MPI_Finalize();
    return 0;
}
