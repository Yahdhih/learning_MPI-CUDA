/* Day 10 — Exercise 2: row communicators of a 2x3 grid.
 *
 * Run with exactly 6 ranks seen as a 2x3 grid:
 *      row = rank / 3, col = rank % 3
 *
 * 1. split WORLD by row (color = row, key = col) → `rowcomm`
 * 2. the row leader (col == 0, i.e. rowcomm rank 0) sets value = 100*row
 * 3. Bcast value inside rowcomm from root 0
 * 4. everyone prints: "world %d (row %d, col %d) got %d"
 *    → row 0 all print 0, row 1 all print 100.
 *
 * Run: mpirun -np 6 ./ex2_grid
 */
#include <mpi.h>
#include <stdio.h>

#define COLS 3

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int wrank, wsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);

    if (wsize != 6) {
        if (wrank == 0) fprintf(stderr, "run with exactly -np 6\n");
        MPI_Finalize();
        return 1;
    }

    int row = wrank / COLS;
    int col = wrank % COLS;

    /* TODO 1: split into rowcomm (color = row, key = col) */
    MPI_Comm rowcomm;

    int value = -1;
    /* TODO 2: the row leader (col == 0) sets value = 100 * row */

    /* TODO 3: Bcast value inside rowcomm, root 0 */

    printf("world %d (row %d, col %d) got %d\n", wrank, row, col, value);

    /* TODO 4: free rowcomm */

    MPI_Finalize();
    return 0;
}
