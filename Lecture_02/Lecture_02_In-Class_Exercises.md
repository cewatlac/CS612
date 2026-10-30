# CS612 Lecture 02 — In-Class Exercises

**Divide-and-Conquer, Recurrences & Selection · Sat 03 Oct 2026** · ungraded

## Opening

What is the best lower bound you can prove, right now, for finding the median of $n$ distinct keys in the comparison model? Why does the decision-tree argument from L01 give almost nothing here?

## A — Classify and solve

For each recurrence: does the Master theorem apply? If so, which case (check regularity where needed)? If not, which tool would you use? Give the $\Theta$ bound.

1. $T(n)=8T(n/2)+n^2$
2. $T(n)=2T(n/4)+\sqrt n$
3. $T(n)=2T(n/2)+n/\log n$
4. $T(n)=3T(n/3)+n\log^2 n$
5. $T(n)=T(n/2)+T(n/4)+n$
6. $T(n)=2T(n/2)+n^2$

## A2 — Substitution practice (if time permits)

Prove by substitution that $T(n)=4T(n/2)+n$ satisfies $T(n)=O(n^2)$. First try the hypothesis $T(n)\le cn^2$ and explain exactly where it fails. Then strengthen it.

## B — Selection

1. In the median-of-medians lemma, why do we exclude "the group containing $x$ and a possibly short last group"? What is the exact bound on $|L|$ if $n$ is a multiple of 10?
2. Redo the analysis for groups of **7**: give the size bound for the larger side and the recurrence, and prove $T(n)=O(n)$ by substitution with an explicit constant.
3. Kadane's algorithm: state the invariant and prove the maintenance step. Modify the algorithm to return the indices of an optimal subarray.

## C — Pick the method (and defend it)

For each scenario give the method, its cost *with the guarantee type*, and the assumption that, if it changed, would change your answer.

1. A public web API returns the median of a client-supplied array ($n\le10^6$) under a strict latency SLO. Attackers may submit crafted arrays.
2. A stream of $10^{10}$ events; report the 100 largest values at the end. Memory: 1 GB.
3. A fixed data set of $n=10^8$ values will receive $10^5$ rank queries with different $k$ over the next hour.
4. An in-memory array of $n=10^9$ values; one report of the 1st, 2nd, …, 99th percentiles.

## Take-home experiment (optional)

Run `Student_Resources/selection_bench.cpp`. (1) Explain why the quickselect comparison count per element stays near 3.4. (2) Construct an input that makes a *deterministic first-element-pivot* quickselect quadratic, and measure it. (3) Try to construct an input on which median-of-medians with groups of 3 shows clear $n\log n$ growth. Is it easy? Why or why not?
