/* Day 03 — Exercise 2 solution. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < size - 1; i++) {
            int value;
            MPI_Status st;
            MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &st);
            printf("got %d from rank %d (tag %d)\n",
                   value, st.MPI_SOURCE, st.MPI_TAG);
        }
    } else {
        int value = rank * 10;
        MPI_Send(&value, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
