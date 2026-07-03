/* Day 10 — Exercise 1: team-wise Allreduce.
 *
 * 1. split WORLD into even/odd teams (color = rank % 2)
 * 2. Allreduce (MPI_SUM) each member's WORLD rank inside the team
 * 3. the member with TEAM rank 0 prints:
 *      "team %d: sum of world ranks = %d"
 *
 * With -np 6: even team = 6 (0+2+4), odd team = 9 (1+3+5).
 * Run: mpirun -np 6 ./ex1_teams
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int wrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    int color = wrank % 2;

    /* TODO 1: split MPI_COMM_WORLD by color (key = wrank) into `team` */
    MPI_Comm team;

    /* TODO 2: get my team rank */
    int trank;

    /* TODO 3: Allreduce wrank with MPI_SUM over `team` into `sum` */
    int sum = 0;

    /* TODO 4: if team rank is 0, print the team's sum */

    /* TODO 5: free the communicator */

    MPI_Finalize();
    return 0;
}
