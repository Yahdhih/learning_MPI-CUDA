/* Day 07 — Exercise 1 solution: parallel pi. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long n = (argc > 1) ? atol(argv[1]) : 100000000L;

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    double h = 1.0 / (double)n;
    double partial = 0.0;

    for (long i = rank; i < n; i += size) {
        double x = h * ((double)i + 0.5);
        partial += 4.0 / (1.0 + x * x);
    }
    partial *= h;

    double pi = 0.0;
    MPI_Reduce(&partial, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double t1 = MPI_Wtime();

    if (rank == 0)
        printf("np=%d  n=%ld  pi=%.15f  error=%.2e  time=%.3f s\n",
               size, n, pi, fabs(pi - M_PI), t1 - t0);

    MPI_Finalize();
    return 0;
}
