/* Day 10 — Exercise 1 solution. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int wrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    int color = wrank % 2;

    MPI_Comm team;
    MPI_Comm_split(MPI_COMM_WORLD, color, wrank, &team);

    int trank;
    MPI_Comm_rank(team, &trank);

    int sum = 0;
    MPI_Allreduce(&wrank, &sum, 1, MPI_INT, MPI_SUM, team);

    if (trank == 0)
        printf("team %d: sum of world ranks = %d\n", color, sum);

    MPI_Comm_free(&team);
    MPI_Finalize();
    return 0;
}
