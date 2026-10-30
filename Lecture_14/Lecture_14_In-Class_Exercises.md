# CS612 Lecture 14 — In-Class Exercises

**Approximation Algorithms, Algorithmic Trade-offs & Course Synthesis · Sat 26 Dec 2026** · ungraded

## A — Guarantees

1. Show that the maximal-matching algorithm is not a 2-approximation for weighted vertex cover. Why does LP rounding still work?
2. **Max-coverage:** choose $k$ sets to cover as many elements as possible. Explain why greedy achieves $\ge(1-1/e)\,\mathrm{OPT}$.
3. A heuristic returned covers of size 102 on 50 benchmark graphs with known optimum 100. What is wrong with calling it a "1.02-approximation"?

## B — Design with guarantees

1. Show that the double-tree algorithm's factor 2 for metric TSP is essentially tight.
2. **$k$-center** (metric): show that farthest-first traversal is a 2-approximation.
3. Knapsack FPTAS with $n=1000$, $\varepsilon=0.01$: estimate the DP table size. Is it practical? What would you do instead?

## C — The whole procedure

EV chargers at intersections: every road segment needs a charger within distance $D$ of both endpoints; site costs differ. (1) Model it and name the classic problem. (2) Input size and key assumptions. (3) Is an exact efficient algorithm plausible? Sketch a reduction. (4) Two responses with their guarantees. (5) An experiment: hypothesis and refutation criterion.

## Take-home (optional)

Run `Student_Resources/tsp_approx_bench.cpp`. Add Or-opt moves or a Christofides implementation (use a greedy matching instead of a minimum-weight one, and explain what guarantee is lost), and compare.
