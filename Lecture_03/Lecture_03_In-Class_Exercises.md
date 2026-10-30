# CS612 Lecture 03 — In-Class Exercises

**Randomized Algorithms & Probabilistic Analysis · Sat 10 Oct 2026** · ungraded

## Opening

(1) What would it mean for a sorting algorithm to be "safe" against an adversary who crafts inputs? (2) What would it mean to "check" a claimed matrix product $C=AB$ in $O(n^2)$ time? In each case, what are we willing to give up?

## A — Classify the guarantee

For each algorithm: Las Vegas or Monte Carlo? One- or two-sided error? What is random and what is bounded?

1. Randomized quicksort
2. Freivalds' algorithm
3. Miller–Rabin primality test
4. Rabin–Karp that reports a match whenever the hashes are equal
5. Rabin–Karp that verifies every hash match character by character
6. Estimating the median as the median of a random sample of 1000 values

## B — Analysis practice

1. In randomized quicksort, compute the probability that $z_1$ and $z_n$ are compared, and the probability that $z_i$ and $z_{i+1}$ are compared. Explain both intuitively.
2. Using the lemma $\Pr[z_i,z_j\text{ compared}]=2/(j-i+1)$, show that $\mathbb E[X]<2nH_n$.
3. (Harder) Prove the three-group bound $\mathbb E[\text{comparisons}]\le 4n+O(1)$ for randomized quickselect.

## C — Design with randomness

1. **Budget.** For $n=10^4$, how many Freivalds trials give error $\le10^{-12}$? Compare the cost with recomputing $AB$.
2. **LV → MC.** Randomized quickselect uses $\le4n$ comparisons in expectation. Give a Monte Carlo version that always stops within $40n$ comparisons, and bound its failure probability. Can you get failure probability $2^{-10}$ within $100n$ comparisons?
3. **Reservoir sampling ($k=1$).** Keep $s=x_1$; for $t\ge2$ replace $s$ by $x_t$ with probability $1/t$. Prove that after $n$ items $\Pr[s=x_i]=1/n$ for every $i$. Generalise to a sample of size $k$.
4. **Adversary.** A web service sorts user-supplied arrays with randomized quicksort, seeded with the server start time; response times are visible. Describe an attack, and a fix.

## Take-home (optional)

Run `Student_Resources/freivalds.cpp`. Modify it to use $r\in\{1,\dots,100\}^n$ and measure the single-trial detection rate for a single wrong entry. What does theory predict?
