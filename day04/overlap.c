/* Day 04 — demo: overlapping communication with computation.
 * Compares (blocking exchange, then compute) vs (Iexchange + compute, Wait).
 * Run: mpirun -np 2 ./overlap
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N (1 << 22)

/* some computation that does not touch the message buffers */
static double busywork(void) {
    double s = 0;
    for (int i = 1; i < 3000000; i++) s += sqrt((double)i);
    return s;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int other = 1 - rank;

    int *mine   = malloc(N * sizeof(int));
    int *theirs = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) mine[i] = rank;

    /* --- blocking version ------------------------------------------------ */
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    MPI_Sendrecv(mine, N, MPI_INT, other, 0,
                 theirs, N, MPI_INT, other, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    double w1 = busywork();
    double t_block = MPI_Wtime() - t0;

    /* --- non-blocking + overlap ------------------------------------------ */
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    MPI_Request reqs[2];
    MPI_Irecv(theirs, N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(mine,   N, MPI_INT, other, 0, MPI_COMM_WORLD, &reqs[1]);
    double w2 = busywork();               /* runs WHILE the network works */
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    double t_nb = MPI_Wtime() - t0;

    if (rank == 0)
        printf("blocking: %.4f s   non-blocking+overlap: %.4f s   (work=%g,%g)\n",
               t_block, t_nb, w1, w2);

    free(mine); free(theirs);
    MPI_Finalize();
    return 0;
}
