/* Day 10 — Exercise 2 solution. */
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

    MPI_Comm rowcomm;
    MPI_Comm_split(MPI_COMM_WORLD, row, col, &rowcomm);

    int value = -1;
    if (col == 0) value = 100 * row;

    MPI_Bcast(&value, 1, MPI_INT, 0, rowcomm);

    printf("world %d (row %d, col %d) got %d\n", wrank, row, col, value);

    MPI_Comm_free(&rowcomm);
    MPI_Finalize();
    return 0;
}
