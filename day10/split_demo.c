/* Day 10 — demo: split WORLD into even/odd teams, show rank mapping.
 * Run: mpirun -np 6 ./split_demo
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int wrank, wsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);

    int color = wrank % 2;                 /* 0 = even team, 1 = odd team */

    MPI_Comm team;
    MPI_Comm_split(MPI_COMM_WORLD, color, wrank, &team);

    int trank, tsize;
    MPI_Comm_rank(team, &trank);
    MPI_Comm_size(team, &tsize);

    printf("world rank %d/%d  →  %s team, team rank %d/%d\n",
           wrank, wsize, color == 0 ? "even" : "odd ", trank, tsize);

    MPI_Comm_free(&team);
    MPI_Finalize();
    return 0;
}
