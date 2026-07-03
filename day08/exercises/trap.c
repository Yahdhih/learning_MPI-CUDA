/* Day 08 — Project: parallel trapezoidal integration of sin(x).
 *
 * Usage: mpirun -np 4 ./trap <a> <b> <n>
 *   e.g. mpirun -np 4 ./trap 0 3.14159265358979 10000000   → 2.0
 *
 * Follow the standard skeleton: parse on root → Bcast → block-compute
 * → Reduce → root prints.
 */
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

    /* TODO 1: rank 0 parses a, b, n from argv (atof/atof/atol) */

    /* TODO 2: broadcast a (MPI_DOUBLE), b (MPI_DOUBLE), n (MPI_LONG) */

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    double h = (b - a) / (double)n;

    /* TODO 3: block distribution with remainder:
     *   per = n / size; rem = n % size;
     *   lo    = rank * per + (rank < rem ? rank : rem);
     *   count = per + (rank < rem ? 1 : 0);
     */
    long lo = 0, count = 0;

    /* TODO 4: local trapezoid sum over [lo, lo+count):
     *   for each trapezoid i: left  x_i = a + i*h, right x_{i+1}
     *   partial += 0.5 * (f(x_i) + f(x_{i+1})) * h;
     * (simple version; you may optimize endpoint-sharing later)
     */
    double partial = 0.0;

    /* TODO 5: Reduce partial into `result` (MPI_SUM, root 0) */
    double result = 0.0;

    double t1 = MPI_Wtime();

    if (rank == 0) {
        double exact = cos(a) - cos(b);       /* integral of sin */
        printf("np=%d n=%ld  integral=%.12f  error=%.2e  time=%.3f s\n",
               size, n, result, fabs(result - exact), t1 - t0);
    }

    MPI_Finalize();
    return 0;
}
