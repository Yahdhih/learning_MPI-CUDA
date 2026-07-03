CC = mpicc
CFLAGS = -Wall -O2

all: trap

trap: exercises/trap.c
	$(CC) $(CFLAGS) $< -o $@ -lm

sol: solutions/trap.c
	$(CC) $(CFLAGS) $< -o trap_sol -lm

clean:
	rm -f trap trap_sol

.PHONY: all sol clean
