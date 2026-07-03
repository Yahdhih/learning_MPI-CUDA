/* Day 07 — Exercise 1: parallel computation of pi.
 *
 * pi = integral of 4/(1+x^2) on [0,1], midpoint rule, n slices.
 * Cyclic distribution: rank r handles i = r, r+size, r+2*size, ...
 *
 * Run: mpirun -np 4 ./ex1_pi 100000000
 * Compare the time with -np 1 and -np 2: this is your first speedup!
 */
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

    MPI_Barrier(MPI_COMM_WORLD);          /* fair start for timing */
    double t0 = MPI_Wtime();

    double h = 1.0 / (double)n;
    double partial = 0.0;

    /* TODO 1: loop i = rank; i < n; i += size
     *   x = h * (i + 0.5);
     *   partial += 4.0 / (1.0 + x*x);
     */

    partial *= h;

    double pi = 0.0;
    /* TODO 2: MPI_Reduce partial into pi with MPI_SUM on root 0 */

    double t1 = MPI_Wtime();

    if (rank == 0)
        printf("np=%d  n=%ld  pi=%.15f  error=%.2e  time=%.3f s\n",
               size, n, pi, fabs(pi - M_PI), t1 - t0);

    MPI_Finalize();
    return 0;
}
