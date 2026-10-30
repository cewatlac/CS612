# CS612 Lecture 12 — In-Class Exercises

**Network Flow, Min-Cut & Matching · Sat 12 Dec 2026** · ungraded

## A — By hand

Network: $s\to a$ (10), $s\to b$ (5), $a\to b$ (15), $a\to c$ (4), $b\to d$ (8), $c\to t$ (10), $d\to c$ (6), $d\to t$ (5).

1. Run Edmonds–Karp: each augmenting path and its bottleneck.
2. Give the reachable set $S$ in the final residual graph and the minimum cut; check that its capacity equals the flow.
3. Which single edge would you upgrade by +5 to increase throughput the most? Justify with cuts.
4. If every capacity is multiplied by 1000, how many augmentations do Edmonds–Karp and an unlucky Ford–Fulkerson rule need?

## B — Model it (prove both directions)

1. **Dinner seating:** families of size $a_i$, tables with $b_j$ seats, no two members of the same family at one table.
2. **Tutoring:** students request subjects (one hour each); tutors teach some subjects and have at most $k_t$ hours. Maximise the hours scheduled.
3. **Evacuation:** corridor capacities, staircase node capacities, several exits: the maximum evacuation rate.

## C — Flow or not?

Can each be solved exactly with polynomially many max-flow computations? Give the model, or explain what breaks.

1. Maximum number of vertex-disjoint $s$–$t$ paths (directed graph).
2. A Hamiltonian $s$–$t$ path.
3. Workers assigned to shifts: each works exactly 3 shifts or none.
4. Global minimum cut of an undirected graph.
5. Maximum matching in a general graph via the "bipartite double" construction with $L=R=V$.
