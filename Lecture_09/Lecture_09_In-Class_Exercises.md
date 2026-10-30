# CS612 Lecture 09 — In-Class Exercises

**Minimum Spanning Trees, DSU & Structural Graph Reasoning · Sat 21 Nov 2026** · ungraded

## Opening

(A) $2\times10^5$ sites, $6\times10^5$ candidate links (sparse). (B) $10^4$ points, all pairs allowed, distances computed on demand (dense, not stored). Which MST algorithm for each, and why? What must each algorithm store and sort?

## A — Choose, justify, adapt

1. Choose an MST algorithm (and priority queue, if any) for: (a) network A; (b) network B; (c) a graph distributed over 64 machines; (d) edges arriving pre-sorted by weight.
2. In a triangle whose edges all have weight 1, show how Borůvka without tie-breaking can create a cycle. Give a consistent tie-break rule.
3. Some edges are required and some forbidden. Adapt Kruskal. When is the problem infeasible?
4. How do you compute a maximum spanning tree? Does the cut property change?

## B — DSU reasoning

1. Kruskal on pre-sorted edges: total cost for $m=10^7$, $n=10^6$? Which term dominates?
2. Rollback DSU for offline dynamic connectivity: why must path compression be disabled? What does each operation cost?
3. Scheduling unit jobs (L08): use DSU to find the latest free slot $\le d_j$. Describe it and give the total cost.
4. With both heuristics, can a single FIND take $\Theta(\log n)$ time? $\Theta(n)$?

## C — MST variants

1. **$k$-clustering with maximum spacing:** prove that Kruskal stopped at $k$ components is optimal.
2. **Incremental MST:** a new edge is added to $G$. Update the MST in $O(n)$ and prove it correct.
3. **Second-best MST** (distinct weights): an $O(n^2)$ algorithm, given the MST.

## Take-home (optional)

Run `Student_Resources/mst_dsu_bench.cpp`. (1) Add a variant of Prim with a pairing heap or with decrease-key, and compare. (2) Find the density at which array-Prim overtakes Kruskal on your machine, and compare with the prediction $m\approx n^2/\log n$.
