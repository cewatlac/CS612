# CS612 Lecture 13 — In-Class Exercises

**NP-Completeness, Reductions & Exact Exponential Algorithms · Sat 19 Dec 2026** · ungraded

## Opening

"No two conflicting nurses share a shift slot; minimise the number of slots." Which classic problem is this? How would you convince a manager that no efficient exact algorithm is known?

## A — Certificates and directions

1. Give a certificate and a verifier (with running time) for: (a) $G$ has a clique of size $\ge k$; (b) the integer $N$ is composite; (c) a knapsack instance has a solution of value $\ge V$.
2. True or false, with a one-line reason: (a) $A\le_p B$ and $A$ NP-hard ⇒ $B$ NP-hard; (b) $A\le_p B$ and $B\in$ P ⇒ $A\in$ P; (c) $A\le_p B$ and $B$ NP-hard ⇒ $A$ NP-hard; (d) every NP-hard problem is in NP; (e) an $O(2^{\sqrt n})$ algorithm for an NP-complete problem would imply P = NP.

## B — Reductions

1. **3-COLOR ≤ 4-COLOR.** Construction and both directions.
2. **HAM-CYCLE ≤ TSP** (decision). Construction. What does it imply about approximating TSP without the triangle inequality?
3. **Find the flaw:** "LONGEST-PATH is NP-hard because I reduced it to 3-SAT."

## C — Exact methods

1. Why is "stop when the partial sum exceeds $T$" invalid for subset sum with negative numbers? Give an instance.
2. Prove that the fractional-knapsack bound is a valid branch-and-bound bound.
3. Derive the recurrence and the base of the exponent for vertex-cover branching on a vertex $v$ of degree $d\ge3$ ("$v$ in the cover, or all of $N(v)$").
4. Estimate the running time of Held–Karp for $n=20$ and $n=40$.
