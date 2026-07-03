/* Day 09 — demo: send a matrix COLUMN with MPI_Type_vector.
 * Run: mpirun -np 2 ./column_demo
 */
#include <mpi.h>
#include <stdio.h>

#define N 6

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double m[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                m[i][j] = 10.0 * i + j;          /* m[i][j] encodes (i,j) */

        MPI_Datatype coltype;
        MPI_Type_vector(N, 1, N, MPI_DOUBLE, &coltype);
        MPI_Type_commit(&coltype);

        /* send column 2: values 2, 12, 22, 32, 42, 52 */
        MPI_Send(&m[0][2], 1, coltype, 1, 0, MPI_COMM_WORLD);

        MPI_Type_free(&coltype);
    } else if (rank == 1) {
        double col[N];   /* received as plain contiguous doubles */
        MPI_Recv(col, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        printf("rank 1 received column:");
        for (int i = 0; i < N; i++) printf(" %.0f", col[i]);
        printf("   (expected 2 12 22 32 42 52)\n");
    }

    MPI_Finalize();
    return 0;
}
