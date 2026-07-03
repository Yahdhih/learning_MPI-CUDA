CC = mpicc
CFLAGS = -Wall -O2

all: pingpong ex1_ring

pingpong: pingpong.c
	$(CC) $(CFLAGS) $< -o $@

ex1_ring: exercises/ex1_ring.c
	$(CC) $(CFLAGS) $< -o $@

sol: solutions/ex1_ring.c
	$(CC) $(CFLAGS) $< -o ex1_ring_sol

clean:
	rm -f pingpong ex1_ring ex1_ring_sol

.PHONY: all sol clean
