/* Day 04 — Exercise 2: non-blocking neighbor shift.
 *
 * Every rank sends its rank to its right neighbor and receives from its
 * left neighbor — all simultaneously, no special case for rank 0:
 *     Irecv(from prev)  ;  Isend(to next)  ;  Waitall
 * Then each rank prints "rank %d received %d" (must equal prev).
 *
 * Run: mpirun -np 4 ./ex2_ring_nb
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    int outgoing = rank;
    int incoming = -1;
    MPI_Request reqs[2];

    /* TODO 1: Irecv `incoming` from prev into reqs[0] */

    /* TODO 2: Isend `outgoing` to next into reqs[1] */

    /* TODO 3: Waitall on both requests */

    printf("rank %d received %d (expected %d)\n", rank, incoming, prev);

    MPI_Finalize();
    return 0;
}
