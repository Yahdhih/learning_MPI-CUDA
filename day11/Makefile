CC = mpicc
CFLAGS = -Wall -O2

all: neighbors ex1_halo

neighbors: neighbors.c
	$(CC) $(CFLAGS) $< -o $@

ex1_halo: exercises/ex1_halo.c
	$(CC) $(CFLAGS) $< -o $@

sol: solutions/ex1_halo.c
	$(CC) $(CFLAGS) $< -o ex1_halo_sol

clean:
	rm -f neighbors ex1_halo ex1_halo_sol

.PHONY: all sol clean
