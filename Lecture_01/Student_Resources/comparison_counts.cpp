// CS612 L01 — How close do real sorting algorithms get to the comparison lower bound ceil(log2 n!)?
// And when does a non-comparison sort (counting sort) win, and when does it lose?
// Build: g++ -std=c++20 -O2 comparison_counts.cpp -o comparison_counts
// Run:   ./comparison_counts            (about 5 seconds)
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

static long long cmp_count = 0;
struct Counted {                       // a key whose comparisons are counted
    int v;
    bool operator<(const Counted& o) const { ++cmp_count; return v < o.v; }
};

double log2_factorial(int n) { return std::lgamma(n + 1.0) / std::log(2.0); }

void insertion_sort(std::vector<Counted>& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        Counted x = a[i]; size_t j = i;
        while (j > 0 && x < a[j - 1]) { a[j] = a[j - 1]; --j; }
        a[j] = x;
    }
}
void merge_sort(std::vector<Counted>& a, std::vector<Counted>& tmp, size_t lo, size_t hi) {
    if (hi - lo < 2) return;
    size_t mid = (lo + hi) / 2;
    merge_sort(a, tmp, lo, mid); merge_sort(a, tmp, mid, hi);
    size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = (a[j] < a[i]) ? a[j++] : a[i++];
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    std::copy(tmp.begin() + lo, tmp.begin() + hi, a.begin() + lo);
}

int main() {
    std::mt19937 rng(612);
    std::printf("Part 1: comparisons on random permutations (mean of 5 runs)\n");
    std::printf("%8s %14s %14s %14s %14s\n", "n", "ceil(lg n!)", "merge sort", "std::sort", "insertion");
    for (int n : {16, 256, 4096, 65536}) {
        double m = 0, s = 0, ins = 0;
        for (int rep = 0; rep < 5; ++rep) {
            std::vector<Counted> a(n);
            for (int i = 0; i < n; ++i) a[i].v = i;
            std::shuffle(a.begin(), a.end(), rng);
            auto b = a, c = a; std::vector<Counted> tmp(n);
            cmp_count = 0; merge_sort(a, tmp, 0, n); m += cmp_count;
            cmp_count = 0; std::sort(b.begin(), b.end()); s += cmp_count;
            if (n <= 4096) { cmp_count = 0; insertion_sort(c); ins += cmp_count; }
        }
        std::printf("%8d %14.0f %14.0f %14.0f %14s\n", n, std::ceil(log2_factorial(n)), m / 5, s / 5,
                    n <= 4096 ? std::to_string((long long)(ins / 5)).c_str() : "(skipped)");
    }

    std::printf("\nPart 2: counting sort vs std::sort, n = 2,000,000 keys in [0, K) (ms, median of 5)\n");
    std::printf("%12s %12s %12s\n", "K", "counting", "std::sort");
    const int n = 2'000'000;
    for (long long K : {256LL, 65'536LL, 2'000'000LL, 64'000'000LL, 256'000'000LL}) {
        std::vector<double> tc, ts;
        for (int rep = 0; rep < 5; ++rep) {
            std::uniform_int_distribution<long long> d(0, K - 1);
            std::vector<int> a(n); for (auto& x : a) x = (int)d(rng);
            auto b = a; std::vector<int> out(n);
            auto t0 = std::chrono::steady_clock::now();
            { std::vector<int> cnt(K + 1, 0);                 // counting sort: Theta(n + K) time and space
              for (int x : a) ++cnt[x + 1];
              for (long long k = 0; k < K; ++k) cnt[k + 1] += cnt[k];
              for (int x : a) out[cnt[x]++] = x; }
            auto t1 = std::chrono::steady_clock::now();
            std::sort(b.begin(), b.end());
            auto t2 = std::chrono::steady_clock::now();
            if (out != b) { std::printf("MISMATCH\n"); return 1; }
            tc.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
            ts.push_back(std::chrono::duration<double, std::milli>(t2 - t1).count());
        }
        std::sort(tc.begin(), tc.end()); std::sort(ts.begin(), ts.end());
        std::printf("%12lld %12.1f %12.1f\n", K, tc[2], ts[2]);
    }
}
