CC = mpicc
CFLAGS = -Wall -O2

all: ex1_pi ex2_minmax

ex1_pi: exercises/ex1_pi.c
	$(CC) $(CFLAGS) $< -o $@ -lm

ex2_minmax: exercises/ex2_minmax.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_pi.c -o ex1_pi_sol -lm
	$(CC) $(CFLAGS) solutions/ex2_minmax.c -o ex2_minmax_sol

clean:
	rm -f ex1_pi ex2_minmax *_sol

.PHONY: all sol clean
