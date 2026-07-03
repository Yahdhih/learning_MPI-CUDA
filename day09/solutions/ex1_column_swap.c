/* Day 09 — Exercise 1 solution. */
#include <mpi.h>
#include <stdio.h>

#define N 6

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double m[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            m[i][j] = rank * 100.0 + 10.0 * i + j;

    double recv[N];

    if (rank == 0) {
        MPI_Datatype coltype;
        MPI_Type_vector(N, 1, N, MPI_DOUBLE, &coltype);
        MPI_Type_commit(&coltype);

        MPI_Send(&m[0][0], 1, coltype, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(recv, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        MPI_Type_free(&coltype);
    } else if (rank == 1) {
        MPI_Recv(recv, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(&m[0][0], N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    printf("rank %d received:", rank);
    for (int i = 0; i < N; i++) printf(" %.0f", recv[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
