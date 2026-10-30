// CS612 L07 — (1) LIS: O(n^2) DP vs O(n log n) patience; (2) 0/1 knapsack: O(nW) is pseudo-polynomial.
// Build: g++ -std=c++20 -O2 dp2_bench.cpp -o dp2     Run: ./dp2   (~20 s)
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }

int lis_quadratic(const std::vector<int>& a) {            // E[i] = LIS ending at i
    std::vector<int> E(a.size(), 1); int best = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < i; ++j) if (a[j] < a[i] && E[j] + 1 > E[i]) E[i] = E[j] + 1;
        best = std::max(best, E[i]);
    }
    return best;
}
int lis_nlogn(const std::vector<int>& a) {                // tail[k] = smallest tail of an increasing subsequence of length k+1
    std::vector<int> tail;
    for (int x : a) { auto it = std::lower_bound(tail.begin(), tail.end(), x); if (it == tail.end()) tail.push_back(x); else *it = x; }
    return (int)tail.size();
}
long long knapsack(const std::vector<long long>& w, const std::vector<long long>& v, long long W) {   // 1-D table, O(nW)
    std::vector<long long> K(W + 1, 0);
    for (size_t i = 0; i < w.size(); ++i)
        for (long long c = W; c >= w[i]; --c) K[c] = std::max(K[c], K[c - w[i]] + v[i]);
    return K[W];
}
int main() {
    std::printf("Part 1: LIS on random permutations\n%9s %12s %12s %8s\n", "n", "O(n^2) ms", "n log n ms", "LIS");
    for (int n : {5000, 20000, 50000, 1000000}) {
        std::vector<int> a(n); for (int i = 0; i < n; ++i) a[i] = i; std::shuffle(a.begin(), a.end(), rng);
        int r1 = -1, r2 = 0; double t1 = -1;
        if (n <= 50000) t1 = ms([&] { r1 = lis_quadratic(a); });
        double t2 = ms([&] { r2 = lis_nlogn(a); });
        if (r1 != -1 && r1 != r2) { std::printf("MISMATCH\n"); return 1; }
        if (t1 < 0) std::printf("%9d %12s %12.1f %8d   (2*sqrt(n) = %.0f)\n", n, "(skipped)", t2, r2, 2 * std::sqrt((double)n));
        else std::printf("%9d %12.1f %12.1f %8d   (2*sqrt(n) = %.0f)\n", n, t1, t2, r2, 2 * std::sqrt((double)n));
    }
    std::printf("\nPart 2: 0/1 knapsack, n = 100 items, weights uniform in [1, W/10], values in [1, 1000]\n");
    std::printf("%14s %8s %12s %14s\n", "capacity W", "bits(W)", "DP ms", "table (MB)");
    for (long long W : {1000LL, 100000LL, 10000000LL, 100000000LL}) {
        std::uniform_int_distribution<long long> dw(1, W / 10), dv(1, 1000);
        std::vector<long long> w(100), v(100); for (int i = 0; i < 100; ++i) { w[i] = dw(rng); v[i] = dv(rng); }
        long long best = 0; double t = ms([&] { best = knapsack(w, v, W); });
        std::printf("%14lld %8d %12.1f %14.1f   (best value %lld)\n", W, (int)std::ceil(std::log2((double)W + 1)), t, (W + 1) * 8 / 1e6, best);
    }
    std::printf("Extrapolation: W = 10^12 (40 bits) would need ~10^4 x the last row's time and 8 TB of table.\n");
}
