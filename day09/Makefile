CC = mpicc
CFLAGS = -Wall -O2

all: column_demo ex1_column_swap ex2_particle

column_demo: column_demo.c
	$(CC) $(CFLAGS) $< -o $@

ex1_column_swap: exercises/ex1_column_swap.c
	$(CC) $(CFLAGS) $< -o $@

ex2_particle: exercises/ex2_particle.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_column_swap.c -o ex1_column_swap_sol
	$(CC) $(CFLAGS) solutions/ex2_particle.c -o ex2_particle_sol

clean:
	rm -f column_demo ex1_column_swap ex2_particle *_sol

.PHONY: all sol clean
