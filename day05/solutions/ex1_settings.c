/* Day 05 — Exercise 1 solution. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int    n     = 0;
    double alpha = 0.0;

    if (rank == 0) {
        n     = 1000;
        alpha = 0.25;
    }

    MPI_Bcast(&n,     1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast(&alpha, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int lo = rank * n / size;
    int hi = (rank + 1) * n / size;
    printf("rank %d: n=%d alpha=%g  my slice = [%d, %d)\n",
           rank, n, alpha, lo, hi);

    MPI_Finalize();
    return 0;
}
