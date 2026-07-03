/* Day 11 — Exercise solution: one halo exchange. */
#include <mpi.h>
#include <stdio.h>

#define NLOC 4

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[1] = {size}, periodic[1] = {0};
    MPI_Comm cart;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periodic, 1, &cart);

    int rank;
    MPI_Comm_rank(cart, &rank);

    int left, right;
    MPI_Cart_shift(cart, 0, 1, &left, &right);

    double u[NLOC + 2];
    u[0] = u[NLOC + 1] = -1.0;
    for (int i = 1; i <= NLOC; i++)
        u[i] = rank * NLOC + (i - 1);

    /* rightward flow: my right edge → right's left ghost */
    MPI_Sendrecv(&u[NLOC], 1, MPI_DOUBLE, right, 0,
                 &u[0],    1, MPI_DOUBLE, left,  0,
                 cart, MPI_STATUS_IGNORE);

    /* leftward flow: my left edge → left's right ghost */
    MPI_Sendrecv(&u[1],        1, MPI_DOUBLE, left,  1,
                 &u[NLOC + 1], 1, MPI_DOUBLE, right, 1,
                 cart, MPI_STATUS_IGNORE);

    printf("rank %d: ghostL=%.0f  own=[%.0f..%.0f]  ghostR=%.0f\n",
           rank, u[0], u[1], u[NLOC], u[NLOC + 1]);

    MPI_Comm_free(&cart);
    MPI_Finalize();
    return 0;
}
