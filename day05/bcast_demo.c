/* Day 05 — demo: rank 0 "reads" a config and broadcasts it to everyone.
 * Run: mpirun -np 4 ./bcast_demo
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* imagine these come from a file or the command line */
    int    nsteps = 0;
    double dt     = 0.0;

    if (rank == 0) {          /* only the root has the real values... */
        nsteps = 500;
        dt     = 0.001;
        printf("rank 0: read config nsteps=%d dt=%g, broadcasting...\n",
               nsteps, dt);
    }

    /* ...and after these two lines, everyone does. */
    MPI_Bcast(&nsteps, 1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast(&dt,     1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    printf("rank %d: nsteps=%d dt=%g\n", rank, nsteps, dt);

    MPI_Finalize();
    return 0;
}
