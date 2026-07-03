/* Day 02 — ping-pong between ranks 0 and 1. Provided, ready to run.
 * Run: mpirun -np 2 ./pingpong
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int counter = 0;
    const int ROUNDS = 10;

    for (int i = 0; i < ROUNDS; i++) {
        if (rank == 0) {
            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            printf("rank 0 got counter = %d\n", counter);
        } else if (rank == 1) {
            MPI_Recv(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            counter++;                                   /* the "pong" */
            MPI_Send(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
