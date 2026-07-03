CC = mpicc
CFLAGS = -Wall -O2

all: deadlock ex1_exchange ex2_anysource

deadlock: deadlock.c
	$(CC) $(CFLAGS) $< -o $@

ex1_exchange: exercises/ex1_exchange.c
	$(CC) $(CFLAGS) $< -o $@

ex2_anysource: exercises/ex2_anysource.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_exchange.c -o ex1_exchange_sol
	$(CC) $(CFLAGS) solutions/ex2_anysource.c -o ex2_anysource_sol

clean:
	rm -f deadlock ex1_exchange ex2_anysource *_sol

.PHONY: all sol clean
