/* Day 09 — Exercise 2 solution. */
#include <mpi.h>
#include <stdio.h>
#include <stddef.h>

typedef struct { int id; double x, y; } Particle;
#define COUNT 5

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int          lens[2]  = {1, 2};
    MPI_Aint     disps[2] = {offsetof(Particle, id), offsetof(Particle, x)};
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};

    MPI_Datatype ptype;
    MPI_Type_create_struct(2, lens, disps, types, &ptype);
    MPI_Type_commit(&ptype);

    Particle p[COUNT];

    if (rank == 0) {
        for (int i = 0; i < COUNT; i++) {
            p[i].id = i; p[i].x = 1.5 * i; p[i].y = -1.0 * i;
        }
        MPI_Send(p, COUNT, ptype, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(p, COUNT, ptype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < COUNT; i++)
            printf("particle id=%d x=%.1f y=%.1f\n", p[i].id, p[i].x, p[i].y);
    }

    MPI_Type_free(&ptype);
    MPI_Finalize();
    return 0;
}
