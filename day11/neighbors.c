/* Day 11 — demo: who are my neighbors in a 1D Cartesian topology?
 * Run: mpirun -np 4 ./neighbors
 * Then change PERIODIC to 1, rebuild, rerun: the line becomes a ring.
 */
#include <mpi.h>
#include <stdio.h>

#define PERIODIC 0

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[1]     = {size};
    int periodic[1] = {PERIODIC};

    MPI_Comm cart;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periodic, 1, &cart);

    int rank;                              /* rank in the NEW communicator */
    MPI_Comm_rank(cart, &rank);

    int left, right;
    MPI_Cart_shift(cart, 0, 1, &left, &right);

    printf("rank %d: left=%2d right=%2d   (%d = MPI_PROC_NULL)\n",
           rank, left, right, MPI_PROC_NULL);

    MPI_Comm_free(&cart);
    MPI_Finalize();
    return 0;
}
