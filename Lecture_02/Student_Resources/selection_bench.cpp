// CS612 L02 — Selection in practice: randomized quickselect vs. median-of-medians vs. sorting vs. std::nth_element.
// Counts key comparisons (via a counting comparator) and measures time. Keys are distinct 64-bit integers.
// Build: g++ -std=c++20 -O2 selection_bench.cpp -o selection_bench     Run: ./selection_bench   (~20 s)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <random>
#include <vector>
using Key = std::int64_t;
static long long CMP = 0;
static inline bool less(Key a, Key b) { ++CMP; return a < b; }
static std::mt19937_64 rng(612);

// Lomuto partition of a[lo..hi] around a[p]; returns final pivot index.
static size_t partition(std::vector<Key>& a, size_t lo, size_t hi, size_t p) {
    std::swap(a[p], a[hi]);
    size_t i = lo;
    for (size_t j = lo; j < hi; ++j) if (less(a[j], a[hi])) std::swap(a[i++], a[j]);
    std::swap(a[i], a[hi]);
    return i;
}
// Randomized quickselect (iterative): returns the k-th smallest (0-based). Seeded once, pivot uniform in [lo,hi].
Key quickselect(std::vector<Key>& a, size_t k) {
    size_t lo = 0, hi = a.size() - 1;
    while (lo < hi) {
        std::uniform_int_distribution<size_t> d(lo, hi);
        size_t q = partition(a, lo, hi, d(rng));
        if (k == q) return a[q];
        if (k < q) hi = q - 1; else lo = q + 1;
    }
    return a[lo];
}
// Deterministic select (BFPRT) with group size G (5 or 3). Returns index of the k-th smallest within a[lo..hi].
size_t mom_select(std::vector<Key>& a, size_t lo, size_t hi, size_t k, int G);
size_t pivot_mom(std::vector<Key>& a, size_t lo, size_t hi, int G) {
    if (hi - lo < (size_t)G) {                           // small: insertion sort, take median
        for (size_t i = lo + 1; i <= hi; ++i) for (size_t j = i; j > lo && less(a[j], a[j - 1]); --j) std::swap(a[j], a[j - 1]);
        return lo + (hi - lo) / 2;
    }
    size_t m = lo;                                        // move group medians to the front
    for (size_t g = lo; g + G - 1 <= hi; g += G) {
        size_t e = g + G - 1;
        for (size_t i = g + 1; i <= e; ++i) for (size_t j = i; j > g && less(a[j], a[j - 1]); --j) std::swap(a[j], a[j - 1]);
        std::swap(a[g + (G - 1) / 2], a[m++]);
    }
    return mom_select(a, lo, m - 1, lo + (m - 1 - lo) / 2, G);
}
size_t mom_select(std::vector<Key>& a, size_t lo, size_t hi, size_t k, int G) {
    while (true) {
        if (lo == hi) return lo;
        size_t p = pivot_mom(a, lo, hi, G);
        size_t q = partition(a, lo, hi, p);
        if (k == q) return q;
        if (k < q) hi = q - 1; else lo = q + 1;
    }
}
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }

int main() {
    std::printf("median selection, distinct keys; comparisons in units of n; time in ms (median of 5)\n");
    std::printf("%-9s %-8s | %9s %9s | %9s %9s | %9s %9s | %9s %9s | %9s\n", "n", "input", "QS cmp/n", "QS ms",
                "MoM5 c/n", "MoM5 ms", "MoM3 c/n", "MoM3 ms", "sort c/n", "sort ms", "nth ms");
    for (size_t n : {100000u, 1000000u, 10000000u}) {
        for (const char* kind : {"random", "sorted"}) {
            std::vector<Key> base(n); std::iota(base.begin(), base.end(), 0);
            if (std::string(kind) == "random") std::shuffle(base.begin(), base.end(), rng);
            size_t k = n / 2;
            double r[5][5]; long long c[4] = {0, 0, 0, 0};
            for (int rep = 0; rep < 5; ++rep) {
                auto a = base; CMP = 0; r[0][rep] = ms([&] { quickselect(a, k); }); c[0] += CMP;
                a = base; CMP = 0; r[1][rep] = ms([&] { mom_select(a, 0, n - 1, k, 5); }); c[1] += CMP;
                a = base; CMP = 0; r[2][rep] = ms([&] { mom_select(a, 0, n - 1, k, 3); }); c[2] += CMP;
                a = base; CMP = 0; r[3][rep] = ms([&] { std::sort(a.begin(), a.end(), less); }); c[3] += CMP;
                a = base; r[4][rep] = ms([&] { std::nth_element(a.begin(), a.begin() + k, a.end()); });
                if (a[k] != (Key)k) { std::printf("WRONG ANSWER\n"); return 1; }
            }
            double med[5]; for (int i = 0; i < 5; ++i) { std::sort(r[i], r[i] + 5); med[i] = r[i][2]; }
            std::printf("%-9zu %-8s | %9.2f %9.1f | %9.2f %9.1f | %9.2f %9.1f | %9.2f %9.1f | %9.1f\n", n, kind,
                        c[0] / 5.0 / n, med[0], c[1] / 5.0 / n, med[1], c[2] / 5.0 / n, med[2], c[3] / 5.0 / n, med[3], med[4]);
        }
    }
}
