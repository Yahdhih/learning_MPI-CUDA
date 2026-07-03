CC = mpicc
CFLAGS = -Wall -O2

all: bcast_demo ex1_settings ex2_guess

bcast_demo: bcast_demo.c
	$(CC) $(CFLAGS) $< -o $@

ex1_settings: exercises/ex1_settings.c
	$(CC) $(CFLAGS) $< -o $@

ex2_guess: exercises/ex2_guess.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_settings.c -o ex1_settings_sol
	$(CC) $(CFLAGS) solutions/ex2_guess.c -o ex2_guess_sol

clean:
	rm -f bcast_demo ex1_settings ex2_guess *_sol

.PHONY: all sol clean
