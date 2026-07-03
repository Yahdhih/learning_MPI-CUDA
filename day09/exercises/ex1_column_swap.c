/* Day 09 — Exercise 1: column for row.
 *
 * Both ranks own a 6x6 matrix filled with m[i][j] = rank*100 + 10*i + j.
 *   - rank 0 sends its COLUMN 0 to rank 1     (needs a vector type)
 *   - rank 1 sends its ROW 0    to rank 0     (already contiguous!)
 * Each receives into a flat double[6] and prints it.
 *
 * Expected: rank 0 prints 100..105 ; rank 1 prints 0 10 20 30 40 50.
 * Run: mpirun -np 2 ./ex1_column_swap
 */
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
        /* TODO 1: create + commit a vector type for a column of m */

        /* TODO 2: send column 0 (&m[0][0], count 1, coltype) to rank 1 */

        /* TODO 3: receive rank 1's row into recv (N plain doubles) */

        /* TODO 4: free the type */
    } else if (rank == 1) {
        /* TODO 5: receive rank 0's column into recv (N plain doubles) */

        /* TODO 6: send row 0 (&m[0][0], N doubles — contiguous!) to 0 */
    }

    printf("rank %d received:", rank);
    for (int i = 0; i < N; i++) printf(" %.0f", recv[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
