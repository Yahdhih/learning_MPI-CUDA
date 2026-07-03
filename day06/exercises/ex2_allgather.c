/* Day 06 — Exercise 2: Allgather.
 *
 * Each rank computes my_val = rank * rank.
 * ONE MPI_Allgather builds vals[size] on EVERY rank.
 * Every rank prints its full copy of the table.
 *
 * Run: mpirun -np 4 ./ex2_allgather   → every rank prints "0 1 4 9"
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int my_val = rank * rank;
    int *vals = malloc(size * sizeof(int));

    /* TODO: one MPI_Allgather call — send my_val (count 1),
     *       receive into vals (count 1 per rank) */

    printf("rank %d sees:", rank);
    for (int i = 0; i < size; i++) printf(" %d", vals[i]);
    printf("\n");

    free(vals);
    MPI_Finalize();
    return 0;
}
