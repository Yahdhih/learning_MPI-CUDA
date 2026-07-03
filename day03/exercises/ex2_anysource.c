/* Day 03 — Exercise 2: wildcards and MPI_Status.
 *
 * Every rank != 0 sends the value (rank * 10) to rank 0, using tag = rank.
 * Rank 0 receives size-1 messages with MPI_ANY_SOURCE / MPI_ANY_TAG and
 * prints for each: value, actual source, actual tag (from the status).
 *
 * Run: mpirun -np 4 ./ex2_anysource   (run it several times: order varies)
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < size - 1; i++) {
            int value;
            MPI_Status st;
            /* TODO 1: receive one int from ANY source with ANY tag,
             *         passing &st as the status */

            /* TODO 2: print value, st.MPI_SOURCE and st.MPI_TAG */
        }
    } else {
        int value = rank * 10;
        /* TODO 3: send `value` to rank 0 with tag = rank */
    }

    MPI_Finalize();
    return 0;
}
