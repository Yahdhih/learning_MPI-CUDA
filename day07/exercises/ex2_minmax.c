/* Day 07 — Exercise 2: global min / max (+ bonus: who had it).
 *
 * 1. each rank draws a random int in [0,999], seeded with srand(rank+1)
 * 2. two MPI_Reduce calls (MPI_MIN and MPI_MAX) to rank 0
 * 3. rank 0 prints global min and max
 * 4. BONUS: use MPI_MINLOC with datatype MPI_2INT on a struct
 *    { int value; int rank; } to also print WHICH rank had the min.
 *
 * Run: mpirun -np 4 ./ex2_minmax
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank + 1);
    int my_val = rand() % 1000;
    printf("rank %d drew %d\n", rank, my_val);

    int gmin, gmax;
    /* TODO 1: reduce my_val with MPI_MIN into gmin (root 0) */

    /* TODO 2: reduce my_val with MPI_MAX into gmax (root 0) */

    if (rank == 0)
        printf("global min = %d, global max = %d\n", gmin, gmax);

    /* BONUS: MPI_MINLOC
     * struct { int val; int loc; } in, out;
     * in.val = my_val; in.loc = rank;
     * MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
     * rank 0: printf("min %d was on rank %d\n", out.val, out.loc);
     */

    MPI_Finalize();
    return 0;
}
