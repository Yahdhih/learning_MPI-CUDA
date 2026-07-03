/* Day 05 — Exercise 2 solution. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int secret = -1;

    if (rank == 0) {
        if (argc < 2) {
            fprintf(stderr, "usage: mpirun -np N ./ex2_guess <secret>\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        secret = atoi(argv[1]);
    }

    MPI_Bcast(&secret, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == secret)
        printf("I am the chosen one (rank %d)\n", rank);

    MPI_Finalize();
    return 0;
}
