# CS612 Lecture 08 — In-Class Exercises

**Greedy Algorithms, Exchange Arguments & Matroids · Sat 14 Nov 2026** · ungraded

## Opening — which greedy rule?

Jobs have processing times $t_j$ and deadlines $d_j$; minimise the maximum lateness. For the rules (1) shortest processing time first and (2) smallest slack $d_j-t_j$ first, give a two-job counterexample. Why might earliest deadline first be correct?

## A — Prove or refute

1. **Fractional knapsack:** decreasing value/weight, the last item fractionally. Prove optimality with an exchange argument.
2. **Refuelling:** range $R$, stations $x_1<\dots<x_n$, destination $D$. Refuel only when the next station is out of range, at the farthest reachable station. Prove it minimises the number of stops (staying ahead).
3. **Covering points:** cover $x_1<\dots<x_n$ with the fewest unit-length closed intervals; greedy places $[p,p+1]$ at the leftmost uncovered point. Prove or refute.
4. **Graph colouring:** colour vertices in order of decreasing degree with the smallest available colour. Optimal?

## B — Huffman

1. Build the Huffman tree for frequencies a:45, b:13, c:12, d:16, e:9, f:5 and compute its cost.
2. Where does the proof of Lemma 2 use Lemma 1?

## C1 — Matroid or not?

Prove exchange or give a counterexample; if it is a matroid, say what GREEDY computes.

1. Matchings in a general graph.
2. Linearly independent column sets of a real matrix.
3. Edge sets of a directed graph with in-degree ≤ 1 at every vertex.
4. Item sets of total size ≤ $W$.
5. Edge sets whose removal leaves a connected graph connected.

## C2 — Find the gap

"For coins $1=c_1<\dots<c_k$, greedy is optimal: if an optimal solution does not use the largest coin $c\le T$, its smaller coins sum to at least $c$, so we can replace some of them by one $c$." Find the false step, give a counterexample, and explain why the step is true for powers of 2.
