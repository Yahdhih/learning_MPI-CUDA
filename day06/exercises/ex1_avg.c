/* Day 06 — Exercise 1: parallel average.
 *
 * 1. root fills data[N] with data[i] = i
 * 2. Scatter into chunk[N/size]
 * 3. each rank sums its chunk into `partial`
 * 4. Gather the partials (count 1 each) to root
 * 5. root sums partials, prints average — expected 7999999.5
 *
 * Run: mpirun -np 4 ./ex1_avg
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 16000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int per = N / size;   /* assume size divides N */

    double *data = NULL;
    if (rank == 0) {
        data = malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) data[i] = (double)i;
    }

    double *chunk = malloc(per * sizeof(double));

    /* TODO 1: scatter `data` into `chunk` (count = per, root = 0) */

    double partial = 0.0;
    /* TODO 2: sum my chunk into `partial` */

    double *partials = NULL;
    if (rank == 0) partials = malloc(size * sizeof(double));

    /* TODO 3: gather each rank's `partial` (count 1) into `partials` */

    if (rank == 0) {
        double total = 0.0;
        /* TODO 4: sum partials, print "average = %f" (total / N) */

        free(partials); free(data);
    }

    free(chunk);
    MPI_Finalize();
    return 0;
}
