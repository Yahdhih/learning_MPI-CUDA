/* Day 05 — Exercise 2: the chosen one.
 *
 * Usage: mpirun -np 4 ./ex2_guess 2
 *
 * 1. rank 0 parses the secret from argv[1] (atoi)
 * 2. broadcast the secret — REMEMBER: every rank must call Bcast!
 * 3. the rank whose number equals the secret prints "I am the chosen one".
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int secret = -1;

    /* TODO 1: rank 0 reads the secret from argv[1] */

    /* TODO 2: broadcast the secret from rank 0 */

    /* TODO 3: if (rank == secret) print "I am the chosen one (rank %d)" */

    MPI_Finalize();
    return 0;
}
