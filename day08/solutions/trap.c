/* Day 08 — Project solution: parallel trapezoidal integration. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double f(double x) { return sin(x); }

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0;
    long   n = 1000000;

    if (rank == 0) {
        if (argc >= 4) {
            a = atof(argv[1]);
            b = atof(argv[2]);
            n = atol(argv[3]);
        } else {
            fprintf(stderr, "usage: ./trap a b n — using defaults\n");
        }
    }

    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_LONG,   0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    double h = (b - a) / (double)n;

    long per = n / size;
    long rem = n % size;
    long lo    = rank * per + (rank < rem ? rank : rem);
    long count = per + (rank < rem ? 1 : 0);

    /* endpoint-sharing optimization: sum interior points once,
       halve only the block edges */
    double partial = 0.5 * (f(a + lo * h) + f(a + (lo + count) * h));
    for (long i = lo + 1; i < lo + count; i++)
        partial += f(a + i * h);
    partial *= h;

    double result = 0.0;
    MPI_Reduce(&partial, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double t1 = MPI_Wtime();

    if (rank == 0) {
        double exact = cos(a) - cos(b);
        printf("np=%d n=%ld  integral=%.12f  error=%.2e  time=%.3f s\n",
               size, n, result, fabs(result - exact), t1 - t0);
    }

    MPI_Finalize();
    return 0;
}
