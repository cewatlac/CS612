// CS612 L14 — Guarantees vs. observed quality on random Euclidean TSP.
// Double-tree (MST + shortcut, a 2-approximation), nearest neighbour (no constant guarantee), 2-opt local search (a heuristic),
// against the exact optimum (Held–Karp, small n) and against the MST lower bound (large n).
// Build: g++ -std=c++20 -O2 tsp_approx_bench.cpp -o tsp     Run: ./tsp   (~30 s)
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <random>
#include <vector>
static std::mt19937_64 rng(612);
using Pts = std::vector<std::pair<double, double>>;
double dist(const Pts& p, int a, int b) { return std::hypot(p[a].first - p[b].first, p[a].second - p[b].second); }
double tour_len(const Pts& p, const std::vector<int>& t) { double s = 0; for (size_t i = 0; i < t.size(); ++i) s += dist(p, t[i], t[(i + 1) % t.size()]); return s; }
std::vector<int> mst_parent(const Pts& p, double* w) {             // Prim, O(n^2)
    int n = p.size(); std::vector<double> key(n, 1e300); std::vector<int> par(n, -1); std::vector<char> in(n, 0); key[0] = 0; *w = 0;
    for (int it = 0; it < n; ++it) { int u = -1; for (int v = 0; v < n; ++v) if (!in[v] && (u < 0 || key[v] < key[u])) u = v;
        in[u] = 1; *w += key[u]; for (int v = 0; v < n; ++v) if (!in[v] && dist(p, u, v) < key[v]) { key[v] = dist(p, u, v); par[v] = u; } }
    return par;
}
std::vector<int> double_tree(const Pts& p, double* mstw) {          // preorder walk of the MST = shortcut Euler tour of the doubled tree
    auto par = mst_parent(p, mstw); int n = p.size(); std::vector<std::vector<int>> ch(n);
    for (int v = 1; v < n; ++v) ch[par[v]].push_back(v);
    std::vector<int> tour; std::function<void(int)> dfs = [&](int u) { tour.push_back(u); for (int c : ch[u]) dfs(c); }; dfs(0);
    return tour;
}
std::vector<int> nearest_neighbour(const Pts& p) {
    int n = p.size(); std::vector<char> used(n, 0); std::vector<int> t{0}; used[0] = 1;
    for (int k = 1; k < n; ++k) { int u = t.back(), best = -1; for (int v = 0; v < n; ++v) if (!used[v] && (best < 0 || dist(p, u, v) < dist(p, u, best))) best = v; used[best] = 1; t.push_back(best); }
    return t;
}
void two_opt(const Pts& p, std::vector<int>& t) {                   // improve until no improving 2-exchange exists
    int n = t.size(); bool improved = true;
    while (improved) { improved = false;
        for (int i = 0; i < n - 1; ++i) for (int j = i + 2; j < n; ++j) { if (i == 0 && j == n - 1) continue;
            int a = t[i], b = t[i + 1], c = t[j], d = t[(j + 1) % n];
            if (dist(p, a, c) + dist(p, b, d) + 1e-12 < dist(p, a, b) + dist(p, c, d)) { std::reverse(t.begin() + i + 1, t.begin() + j + 1); improved = true; } } }
}
double held_karp(const Pts& p) {                                    // O(n^2 2^n)
    int n = p.size(), F = 1 << (n - 1); std::vector<double> C((size_t)F * (n - 1), 1e300);
    for (int j = 0; j < n - 1; ++j) C[(size_t)(1 << j) * (n - 1) + j] = dist(p, n - 1, j);
    for (int S = 1; S < F; ++S) for (int j = 0; j < n - 1; ++j) if (S >> j & 1) { double c = C[(size_t)S * (n - 1) + j]; if (c >= 1e299) continue;
        for (int k = 0; k < n - 1; ++k) if (!(S >> k & 1)) { double& x = C[(size_t)(S | 1 << k) * (n - 1) + k]; x = std::min(x, c + dist(p, j, k)); } }
    double best = 1e300; for (int j = 0; j < n - 1; ++j) best = std::min(best, C[(size_t)(F - 1) * (n - 1) + j] + dist(p, j, n - 1)); return best;
}
Pts random_pts(int n) { std::uniform_real_distribution<double> U(0, 1); Pts p(n); for (auto& q : p) q = {U(rng), U(rng)}; return p; }
int main() {
    std::printf("Part 1: n = 12 random points in the unit square, 100 instances: ratio to the exact optimum (Held-Karp)\n");
    std::printf("%-22s %10s %10s\n", "algorithm", "mean", "worst");
    double s1 = 0, s2 = 0, s3 = 0, w1 = 0, w2 = 0, w3 = 0; const int R = 100;
    for (int r = 0; r < R; ++r) { Pts p = random_pts(12); double opt = held_karp(p), mw;
        double a = tour_len(p, double_tree(p, &mw)) / opt, b = tour_len(p, nearest_neighbour(p)) / opt;
        auto t = nearest_neighbour(p); two_opt(p, t); double c = tour_len(p, t) / opt;
        s1 += a; s2 += b; s3 += c; w1 = std::max(w1, a); w2 = std::max(w2, b); w3 = std::max(w3, c); }
    std::printf("%-22s %10.3f %10.3f   (guarantee: 2)\n%-22s %10.3f %10.3f   (no constant guarantee)\n%-22s %10.3f %10.3f   (heuristic)\n",
                "double tree (MST)", s1 / R, w1, "nearest neighbour", s2 / R, w2, "NN + 2-opt", s3 / R, w3);
    std::printf("\nPart 2: n = 1000 random points: tour length / MST weight (MST <= OPT, so these are upper bounds on the ratio to OPT)\n");
    Pts p = random_pts(1000); double mw; auto dt = double_tree(p, &mw); auto nn = nearest_neighbour(p); auto nn2 = nn; two_opt(p, nn2);
    std::printf("  MST weight %.3f | double tree %.3f (x%.3f) | NN %.3f (x%.3f) | NN+2-opt %.3f (x%.3f)\n", mw, tour_len(p, dt), tour_len(p, dt) / mw,
                tour_len(p, nn), tour_len(p, nn) / mw, tour_len(p, nn2), tour_len(p, nn2) / mw);
}
