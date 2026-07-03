CC = mpicc
CFLAGS = -Wall -O2

all: overlap ex1_iexchange ex2_ring_nb

overlap: overlap.c
	$(CC) $(CFLAGS) $< -o $@ -lm

ex1_iexchange: exercises/ex1_iexchange.c
	$(CC) $(CFLAGS) $< -o $@

ex2_ring_nb: exercises/ex2_ring_nb.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_iexchange.c -o ex1_iexchange_sol
	$(CC) $(CFLAGS) solutions/ex2_ring_nb.c -o ex2_ring_nb_sol

clean:
	rm -f overlap ex1_iexchange ex2_ring_nb *_sol

.PHONY: all sol clean
