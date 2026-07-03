/* Day 09 — Exercise 2: send structs with MPI_Type_create_struct.
 *
 * rank 0 fills 5 Particles {id=i, x=i*1.5, y=-i} and sends the array;
 * rank 1 receives and prints them.
 *
 * Run: mpirun -np 2 ./ex2_particle
 */
#include <mpi.h>
#include <stdio.h>
#include <stddef.h>   /* offsetof */

typedef struct { int id; double x, y; } Particle;
#define COUNT 5

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* TODO 1: build the datatype:
     *   lens  = {1, 2}
     *   disps = {offsetof(Particle, id), offsetof(Particle, x)}
     *   types = {MPI_INT, MPI_DOUBLE}
     *   MPI_Type_create_struct(2, ...) + commit
     */
    MPI_Datatype ptype;

    Particle p[COUNT];

    if (rank == 0) {
        for (int i = 0; i < COUNT; i++) {
            p[i].id = i; p[i].x = 1.5 * i; p[i].y = -1.0 * i;
        }
        /* TODO 2: send the COUNT particles to rank 1 (count=COUNT, ptype) */
    } else if (rank == 1) {
        /* TODO 3: receive COUNT particles from rank 0 */

        for (int i = 0; i < COUNT; i++)
            printf("particle id=%d x=%.1f y=%.1f\n", p[i].id, p[i].x, p[i].y);
    }

    /* TODO 4: free the type */

    MPI_Finalize();
    return 0;
}
