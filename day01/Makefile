CC = mpicc
CFLAGS = -Wall -O2

all: hello ex1_roles

hello: hello.c
	$(CC) $(CFLAGS) $< -o $@

ex1_roles: exercises/ex1_roles.c
	$(CC) $(CFLAGS) $< -o $@

sol: solutions/ex1_roles.c
	$(CC) $(CFLAGS) $< -o ex1_roles_sol

clean:
	rm -f hello ex1_roles ex1_roles_sol

.PHONY: all sol clean
