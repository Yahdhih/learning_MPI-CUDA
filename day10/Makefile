CC = mpicc
CFLAGS = -Wall -O2

all: split_demo ex1_teams ex2_grid

split_demo: split_demo.c
	$(CC) $(CFLAGS) $< -o $@

ex1_teams: exercises/ex1_teams.c
	$(CC) $(CFLAGS) $< -o $@

ex2_grid: exercises/ex2_grid.c
	$(CC) $(CFLAGS) $< -o $@

sol:
	$(CC) $(CFLAGS) solutions/ex1_teams.c -o ex1_teams_sol
	$(CC) $(CFLAGS) solutions/ex2_grid.c -o ex2_grid_sol

clean:
	rm -f split_demo ex1_teams ex2_grid *_sol

.PHONY: all sol clean
