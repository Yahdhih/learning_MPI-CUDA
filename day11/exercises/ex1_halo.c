/* Day 11 — Exercise: one halo exchange.
 *
 * Each rank owns NLOC=4 cells with global values rank*4 .. rank*4+3,
 * stored in u[1..NLOC]; u[0] and u[NLOC+1] are ghost cells.
 *
 * Do ONE halo exchange with two MPI_Sendrecv calls:
 *   → rightward flow: send my RIGHT edge u[NLOC] to `right`,
 *                     receive my LEFT ghost u[0] from `left`
 *   ← leftward flow:  send my LEFT edge u[1] to `left`,
 *                     receive my RIGHT ghost u[NLOC+1] from `right`
 *
 * Border ranks have MPI_PROC_NULL neighbors → same code, no ifs;
 * their ghost stays at the sentinel value -1.
 *
 * Run: mpirun -np 4 ./ex1_halo
 * Expected e.g. rank 1: ghostL=3 (rank0's edge), ghostR=8 (rank2's edge).
 */
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
    /* TODO 1: get left/right with MPI_Cart_shift */

    double u[NLOC + 2];
    u[0] = u[NLOC + 1] = -1.0;                  /* ghost sentinels */
    for (int i = 1; i <= NLOC; i++)
        u[i] = rank * NLOC + (i - 1);           /* global cell values */

    /* TODO 2: rightward flow — one MPI_Sendrecv:
     *   send &u[NLOC] to right / recv &u[0] from left (tag 0) */

    /* TODO 3: leftward flow — one MPI_Sendrecv:
     *   send &u[1] to left / recv &u[NLOC+1] from right (tag 1) */

    printf("rank %d: ghostL=%.0f  own=[%.0f..%.0f]  ghostR=%.0f\n",
           rank, u[0], u[1], u[NLOC], u[NLOC + 1]);

    MPI_Comm_free(&cart);
    MPI_Finalize();
    return 0;
}
