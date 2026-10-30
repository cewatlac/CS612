// CS612 L03 — Freivalds' algorithm: verify a claimed matrix product C = A·B in O(n^2) per trial (Monte Carlo, one-sided error).
// Build: g++ -std=c++20 -O2 freivalds.cpp -o freivalds      Run: ./freivalds   (~5 s)
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>
using Mat = std::vector<std::int64_t>;               // row-major n x n
static std::mt19937_64 rng(2026);                    // seeded ONCE

Mat multiply(const Mat& A, const Mat& B, int n) {    // classical O(n^3)
    Mat C(n * n, 0);
    for (int i = 0; i < n; ++i) for (int k = 0; k < n; ++k) { auto a = A[i * n + k];
        for (int j = 0; j < n; ++j) C[i * n + j] += a * B[k * n + j]; }
    return C;
}
// One trial: pick r in {0,1}^n uniformly; accept iff A(Br) == Cr.  If C != AB, rejects with probability >= 1/2.
bool trial(const Mat& A, const Mat& B, const Mat& C, int n) {
    std::vector<std::int64_t> r(n), Br(n, 0), ABr(n, 0), Cr(n, 0);
    for (auto& x : r) x = rng() & 1;
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) { Br[i] += B[i * n + j] * r[j]; Cr[i] += C[i * n + j] * r[j]; }
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) ABr[i] += A[i * n + j] * Br[j];
    return ABr == Cr;
}
bool freivalds(const Mat& A, const Mat& B, const Mat& C, int n, int k) {
    for (int t = 0; t < k; ++t) if (!trial(A, B, C, n)) return false;   // "definitely wrong"
    return true;                                                         // "probably right": error <= 2^-k
}
int main() {
    const int n = 600;
    std::uniform_int_distribution<int> d(-9, 9);
    Mat A(n * n), B(n * n); for (auto& x : A) x = d(rng); for (auto& x : B) x = d(rng);
    auto t0 = std::chrono::steady_clock::now();
    Mat C = multiply(A, B, n);
    auto t1 = std::chrono::steady_clock::now();
    bool ok = freivalds(A, B, C, n, 30);
    auto t2 = std::chrono::steady_clock::now();
    std::printf("n = %d: classical multiply %.1f ms; Freivalds with k = 30 trials %.1f ms; verdict on correct C: %s\n", n,
        std::chrono::duration<double, std::milli>(t1 - t0).count(), std::chrono::duration<double, std::milli>(t2 - t1).count(),
        ok ? "accept" : "REJECT");
    // Empirical single-trial detection rate for two kinds of wrong C.
    const int T = 2000;
    Mat C1 = C; C1[123 * n + 456] += 1;                  // one corrupted entry: detection probability exactly 1/2
    Mat C2 = C; for (int i = 0; i < n; ++i) C2[i * n + (i * 7) % n] += 1;   // one wrong entry in every row
    int det1 = 0, det2 = 0;
    for (int t = 0; t < T; ++t) { det1 += !trial(A, B, C1, n); det2 += !trial(A, B, C2, n); }
    std::printf("single-trial detection rate over %d trials: one wrong entry %.3f (theory 0.5); one wrong entry per row %.3f (theory 1 - 2^-%d)\n",
        T, det1 / (double)T, det2 / (double)T, n);
}
