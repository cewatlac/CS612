# CS612 Lecture 04 — In-Class Exercises

**Hashing & Randomized Data Structures · Sat 17 Oct 2026** · ungraded

## Opening

In the 2011 hash-flooding attacks, the attacked hash tables were "$O(1)$ expected". Which assumption behind that claim did the attacker violate? What exactly must a fix randomize?

## A — Attack and defend

1. A table uses $m=1000$ and $h(k)=k\bmod1000$. Give 100 distinct keys that all collide. Then give a key set that looks "natural" but still causes heavy collisions.
2. With $h_{a,b}(k)=((ak+b)\bmod p)\bmod m$ ($p=10007$, $m=1000$), compute the expected number of colliding pairs among $n=1000$ fixed keys, and the expected number of keys sharing a slot with a given key $x$.
3. A developer picks $a,b$ at random once and hard-codes them into an open-source library. Is the table still protected? Against whom?
4. Why is $h(k)=(ak\bmod p)\bmod m$ with a fixed $a$ not universal?

## B — Quick derivations

1. FKS: prove $\sum_j n_j^2=n+2\cdot\#\{\text{colliding pairs}\}$, and use it to show $\mathbb E[\sum_j n_j^2]<2n$ when $m=n$.
2. Skip list with promotion probability $p=\frac14$: expected pointers per key? Expected height for $n=10^6$?
3. Why does the skip-list analysis fail if each key's level is a deterministic function of the key (e.g. its number of trailing zero bits)?
4. A vendor sells a skip list with "worst-case $O(\log n)$ search". What would you ask?

## C — Choose the structure

For each scenario: the structure, its guarantee with type, the assumption that guarantee needs, and what would change your choice.

1. A rate limiter keyed by client IP; attackers control source addresses; 1M active keys; p99 latency budget 50 µs.
2. A game leaderboard: 10M players, score updates at 50k/s, queries "rank of player X" and "players ranked 1000–1100".
3. A spell checker's dictionary: 500k words loaded once, $10^8$ lookups per day.
4. An in-memory ordered index shared by 32 threads with frequent inserts and range scans.

## Take-home (optional)

Run `Student_Resources/hashing_skiplist_bench.cpp`. (1) Add a key set on which multiply-shift performs worse than on random keys. Can you find one without knowing $a$? (2) Change the skip list to $p=\frac14$ and compare height, steps and time.
