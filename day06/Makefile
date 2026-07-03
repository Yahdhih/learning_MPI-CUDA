CC = mpicc
CFLAGS = -Wall -O2

all: scatter_demo ex1_avg ex2_allgather

scatter_demo: scatter_demo.c
	$(CC) $(CFLAGS) $< -o $@

ex1_avg: exercises/ex1_avg.c
	$(CC) $(CFLAGS) $< -o $@

ex2_allgather: exercises/ex2_allgather.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_avg.c -o ex1_avg_sol
	$(CC) $(CFLAGS) solutions/ex2_allgather.c -o ex2_allgather_sol

clean:
	rm -f scatter_demo ex1_avg ex2_allgather *_sol

.PHONY: all sol clean
