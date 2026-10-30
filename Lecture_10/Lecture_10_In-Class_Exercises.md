# CS612 Lecture 10 — In-Class Exercises

**Shortest Paths as an Algorithm-Selection Problem · Sat 28 Nov 2026** · ungraded

## Opening

For each setting, is Dijkstra **wrong** or merely **slower than necessary**? (1) hops in a courier network; (2) earliest completion of a task plan (a DAG, longest path); (3) road travel times; (4) costs with fuel rebates (negative edges); (5) currency conversions (arbitrage = negative cycle).

## A — Select and justify

1. **Arbitrage** among 150 currencies with rates $r_{ij}$: model it and choose the algorithm.
2. **Grid game** $2000\times2000$, entering a cell costs 0 or 1: minimum cost from corner to corner.
3. **Build plan**: 50,000 tasks with durations and dependencies: the earliest completion time.
4. **Continental routing**: $2\times10^7$ junctions, millions of queries per hour.

## B — Reasoning with assumptions

1. Bellman–Ford with early termination when the shortest-path tree has depth $d$: how many rounds? Can edge order make it faster?
2. Johnson on a graph with no negative edges: is step 1 needed, and what potential does it compute?
3. Floyd–Warshall with $(\lor,\land)$ instead of $(\min,+)$: what does it compute? Name other semirings that fit the same loops.
4. Shortest paths with at most $k$ edges: modify Bellman–Ford. What bug must you avoid inside a round?

## C — Design with assumptions

1. Weekday-only vertex tolls $t_v\ge0$: preprocess or adapt at query time?
2. The most reliable path when edges fail independently with probability $p_e$: reduce it to shortest paths.
3. The widest (maximum-bottleneck) path: modify Dijkstra. What replaces "non-negative weights" in the proof?
4. Design an experiment to decide between Dial and a binary heap for road networks with integer travel times in seconds. State the hypothesis and what would refute it.
