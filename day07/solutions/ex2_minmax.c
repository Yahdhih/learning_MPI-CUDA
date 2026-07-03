/* Day 07 — Exercise 2 solution (with MINLOC bonus). */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank + 1);
    int my_val = rand() % 1000;
    printf("rank %d drew %d\n", rank, my_val);

    int gmin, gmax;
    MPI_Reduce(&my_val, &gmin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&my_val, &gmax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("global min = %d, global max = %d\n", gmin, gmax);

    /* bonus: who had the min */
    struct { int val; int loc; } in, out;
    in.val = my_val;
    in.loc = rank;
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("min %d was on rank %d\n", out.val, out.loc);

    MPI_Finalize();
    return 0;
}
