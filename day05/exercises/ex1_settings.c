/* Day 05 — Exercise 1: broadcast settings, compute my slice.
 *
 * 1. rank 0 sets n = 1000, alpha = 0.25
 * 2. broadcast n and alpha to all ranks
 * 3. every rank prints them and its slice [lo, hi):
 *        lo = rank * n / size;   hi = (rank + 1) * n / size;
 *
 * Run: mpirun -np 4 ./ex1_settings
 * Expected with 4 ranks: slices [0,250) [250,500) [500,750) [750,1000)
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int    n     = 0;
    double alpha = 0.0;

    /* TODO 1: rank 0 sets n = 1000 and alpha = 0.25 */

    /* TODO 2: broadcast n (MPI_INT) and alpha (MPI_DOUBLE) from root 0 */

    /* TODO 3: compute lo and hi, print:
     *   "rank %d: n=%d alpha=%g  my slice = [%d, %d)" */

    MPI_Finalize();
    return 0;
}
