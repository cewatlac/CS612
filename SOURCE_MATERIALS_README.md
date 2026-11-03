# CS612 Weekly Lectures — student materials

Each `Lecture_NN/` folder contains **only student-safe files**:

| File | What it is |
|---|---|
| `Lecture_NN_Slides.pdf` | The slides, no instructor notes. **This is what goes to Google Classroom.** |
| `Lecture_NN_Slides.html` | The same slides as an offline HTML deck (needs the `_shared/` folder next to it) |
| `Lecture_NN_In-Class_Exercises.md` / `.pdf` | Exercise sheet used in Block B/C |
| `Student_Resources/` | Code or data used in class (where applicable) |

The slide files are **generated** from the instructor sources in `Instructor_Only/Lectures/` by `python tools/build.py lectures`, which strips every instructor note and refuses to write a file if note content leaks through. Do not edit them by hand.

| L | Date | Lecture |
|---:|---|---|
| 01 | Sat 26 Sep 2026 | Advanced Algorithm Analysis, Correctness & Lower Bounds |
| 02 | Sat 03 Oct 2026 | Divide-and-Conquer, Recurrences & Selection |
| 03 | Sat 10 Oct 2026 | Randomized Algorithms & Probabilistic Analysis |
| 04 | Sat 17 Oct 2026 | Hashing & Randomized Data Structures |
| 05 | Sat 24 Oct 2026 | Augmented Data Structures & Amortized Analysis |
| 06 | Sat 31 Oct 2026 | Dynamic Programming I — State-Space Design |
| 07 | Sat 07 Nov 2026 | Dynamic Programming II — Optimization & Pseudo-Polynomial Algorithms |
| 08 | Sat 14 Nov 2026 | Greedy Algorithms, Exchange Arguments & Matroids |
| 09 | Sat 21 Nov 2026 | Minimum Spanning Trees, DSU & Structural Graph Reasoning |
| 10 | Sat 28 Nov 2026 | Shortest Paths as an Algorithm-Selection Problem |
| 11 | Sat 05 Dec 2026 | String Algorithms & Multi-Pattern Matching |
| 12 | Sat 12 Dec 2026 | Network Flow, Min-Cut & Matching |
| 13 | Sat 19 Dec 2026 | NP-Completeness, Reductions & Exact Exponential Algorithms |
| 14 | Sat 26 Dec 2026 | Approximation Algorithms, Algorithmic Trade-offs & Course Synthesis |

`_shared/` holds the slide engine (`slides.js`, `styles.css`, `print.css`) and a vendored copy of KaTeX 0.16.22 (MIT licence, `katex/LICENSE`) so that mathematics renders without internet access.
