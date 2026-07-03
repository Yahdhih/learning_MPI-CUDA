/* Day 04 — Exercise 2 solution. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    int outgoing = rank;
    int incoming = -1;
    MPI_Request reqs[2];

    MPI_Irecv(&incoming, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&outgoing, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);

    printf("rank %d received %d (expected %d)\n", rank, incoming, prev);

    MPI_Finalize();
    return 0;
}
