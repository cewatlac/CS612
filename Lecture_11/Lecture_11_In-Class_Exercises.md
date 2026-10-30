# CS612 Lecture 11 — In-Class Exercises

**String Algorithms & Multi-Pattern Matching · Sat 05 Dec 2026** · ungraded

## A — Single pattern

1. Compute $\pi$ for $P=$ `abcabcabd` and list the border chain of $P[0..7]$.
2. Give a text on which KMP performs a long run of fallbacks on a single character. How long can it be for $|P|=m$?
3. Rabin–Karp with $p=2^{31}-1$, $m=50$, $n=10^9$: bound the probability of any false hit. Is it acceptable for a search engine? What would you change?
4. Show that computing $\pi$ of the string $P\#T$ (the separator $\#$ occurs in neither) finds all occurrences of $P$ in $T$. Why is the separator needed?

## B — Aho–Corasick

1. Build the trie and failure links for {`a`, `ab`, `bab`, `bc`, `bca`, `c`, `caa`}. Which nodes have non-trivial output links?
2. Trace the automaton on `abccab` and list every reported (position, pattern).
3. Why are output links needed? Give an example.

## C — Choose and justify

For each: the algorithm/index, its guarantee (deterministic? expected?), memory, and the assumption that would change your choice.

1. Plagiarism screening: do two 200-page documents share a passage of ≥ 50 consecutive words?
2. A fixed 3 GB genome; millions of exact-match queries of length 100 per day.
3. An intrusion-detection sensor: 20,000 byte signatures, 10 Gbit/s, a worst-case per-byte budget.
4. A text editor's "find" in a 5 MB file.

## Take-home (optional)

Run `Student_Resources/strings_bench.cpp`. (1) Add Boyer–Moore–Horspool and compare it with naive and KMP on the three text types. (2) Make Rabin–Karp use a **fixed** base modulo $2^{64}$ and build Thue–Morse text/pattern pairs that collide. How many false hits do you get?
