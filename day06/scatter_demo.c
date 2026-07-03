/* Day 06 — demo: scatter 8 ints over 4 ranks, double them, gather back.
 * Run: mpirun -np 4 ./scatter_demo
 */
#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) fprintf(stderr, "run with a divisor of %d ranks\n", N);
        MPI_Finalize();
        return 1;
    }
    int per = N / size;

    int big[N];
    if (rank == 0)
        for (int i = 0; i < N; i++) big[i] = i + 1;      /* 1..8 */

    int chunk[N];  /* only `per` entries used */
    MPI_Scatter(big, per, MPI_INT, chunk, per, MPI_INT, 0, MPI_COMM_WORLD);

    printf("rank %d got:", rank);
    for (int i = 0; i < per; i++) printf(" %d", chunk[i]);
    printf("\n");

    for (int i = 0; i < per; i++) chunk[i] *= 2;         /* the "work" */

    MPI_Gather(chunk, per, MPI_INT, big, per, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("root gathered:");
        for (int i = 0; i < N; i++) printf(" %d", big[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
