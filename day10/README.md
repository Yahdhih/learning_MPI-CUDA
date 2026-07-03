# Day 10 — Communicators & Groups · `MPI_Comm_split`

## 🎯 Objectives
- Understand what a communicator really is.
- Split `MPI_COMM_WORLD` into sub-teams with `MPI_Comm_split`.
- Run collectives *inside* a sub-team.

## 📖 Theory (20 min)

### What a communicator really is
A communicator = **a group of processes + a private communication
context**. Two facts follow:
1. Ranks are *relative to the communicator*: the same process can be rank 5
   in `MPI_COMM_WORLD` and rank 1 in a sub-communicator.
2. Messages in different communicators can never match each other — a
   library using its own communicator can't collide with your tags.

Why sub-communicators? Because **collectives operate on a whole
communicator**. Want to Bcast only within "row 2 of my process grid"?
You need a communicator containing exactly those ranks.

### `MPI_Comm_split` — the workhorse
```c
MPI_Comm newcomm;
MPI_Comm_split(MPI_COMM_WORLD,
               color,   /* processes with the SAME color end up together */
               key,     /* orders ranks inside the new comm (ties: old rank) */
               &newcomm);
```
Every rank calls it once; each receives the communicator of *its own*
group. A rank can opt out with `MPI_UNDEFINED` as color (gets
`MPI_COMM_NULL`).

Example — split into even/odd teams:
```c
int color = rank % 2;
MPI_Comm team;
MPI_Comm_split(MPI_COMM_WORLD, color, rank, &team);

int team_rank, team_size;
MPI_Comm_rank(team, &team_rank);      /* my NEW rank inside my team */
MPI_Comm_size(team, &team_size);

/* collectives now stay inside the team: */
MPI_Allreduce(&x, &sum, 1, MPI_DOUBLE, MPI_SUM, team);

MPI_Comm_free(&team);                  /* cleanup, like Type_free */
```

### Mental model for a 2D grid (used on day 11)
With 6 processes seen as a 2×3 grid: `row = rank / 3`, `col = rank % 3`.
- split with `color = row` → 2 row-communicators of 3,
- split with `color = col` → 3 column-communicators of 2.
This is how real codes do row-wise / column-wise operations on matrices.

## 💻 Practice (35 min)

1. Run `split_demo.c` with 6 ranks and study the world-rank ↔ team-rank
   mapping it prints.
2. **Exercise 1** — `exercises/ex1_teams.c`: split into even/odd teams;
   each team computes the **sum of its members' world ranks** with an
   `Allreduce` on the team communicator. With `-np 6`: even team → 0+2+4=6,
   odd team → 1+3+5=9. Team-rank-0 of each team prints the result.
3. **Exercise 2** — `exercises/ex2_grid.c`: with 6 ranks as a 2×3 grid,
   create row communicators; the row leader (col 0) broadcasts a value
   `100*row` to its row; everyone prints what they got.

## ✅ Self-check (5 min)
1. A process is rank 4 in WORLD and gets color 0, key 0 in a split with
   ranks {0,2,4}. What's its new rank?
2. Why can't you just use tags to separate "teams"?
3. What must every rank of the parent communicator do when one of them
   calls `MPI_Comm_split`?

<details><summary>Answers</summary>

1. 2 — keys tie, so old-rank order is kept: 0→0, 2→1, 4→2.
2. Collectives don't have tags, and tag discipline doesn't compose across
   libraries. Contexts give real isolation.
3. Also call `MPI_Comm_split` — it is itself a collective over the parent.
</details>
