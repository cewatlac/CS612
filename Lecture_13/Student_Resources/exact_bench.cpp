// CS612 L13 — Exact exponential algorithms in practice.
// (1) 0/1 knapsack: plain backtracking (feasibility pruning only) vs. branch-and-bound with the fractional (LP) bound vs. O(nW) DP.
// (2) Vertex cover of size <= k: branching on an edge (2^k) vs. branching on a vertex of max degree (N(v) or v).
// Build: g++ -std=c++20 -O2 exact_bench.cpp -o exact     Run: ./exact   (~1-2 min)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <random>
#include <vector>
using ll = long long;
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }

struct Knap { std::vector<ll> w, v; ll W; ll best = 0; long long nodes = 0; };
void backtrack(Knap& K, int i, ll cw, ll cv) {                       // feasibility pruning only
    ++K.nodes; if (cv > K.best) K.best = cv; if (i == (int)K.w.size()) return;
    if (cw + K.w[i] <= K.W) backtrack(K, i + 1, cw + K.w[i], cv + K.v[i]);
    backtrack(K, i + 1, cw, cv);
}
double frac_bound(const Knap& K, int i, ll cw, ll cv) {              // items sorted by density: greedy fractional completion
    double b = cv; ll room = K.W - cw;
    for (int j = i; j < (int)K.w.size(); ++j) { if (K.w[j] <= room) { room -= K.w[j]; b += K.v[j]; } else { b += (double)K.v[j] * room / K.w[j]; break; } }
    return b;
}
void bnb(Knap& K, int i, ll cw, ll cv) {
    ++K.nodes; if (cv > K.best) K.best = cv; if (i == (int)K.w.size()) return;
    if (frac_bound(K, i, cw, cv) <= K.best) return;                 // the bound cannot beat the incumbent: prune
    if (cw + K.w[i] <= K.W) bnb(K, i + 1, cw + K.w[i], cv + K.v[i]);
    bnb(K, i + 1, cw, cv);
}
ll dp(const Knap& K) {
    std::vector<ll> T(K.W + 1, 0);
    for (size_t i = 0; i < K.w.size(); ++i)
        for (ll c = K.W; c >= K.w[i]; --c) T[c] = std::max(T[c], T[c - K.w[i]] + K.v[i]);
    return T[K.W];
}
Knap make(int n, bool correlated) {
    Knap K; std::uniform_int_distribution<ll> d(1, 1000);
    for (int i = 0; i < n; ++i) { ll w = d(rng); K.w.push_back(w); K.v.push_back(correlated ? w + 100 : d(rng)); }
    K.W = std::accumulate(K.w.begin(), K.w.end(), 0LL) / 2;
    std::vector<int> id(n); std::iota(id.begin(), id.end(), 0);                 // sort by density (needed by the bound)
    std::sort(id.begin(), id.end(), [&](int a, int b) { return K.v[a] * K.w[b] > K.v[b] * K.w[a]; });
    Knap S; S.W = K.W; for (int i : id) { S.w.push_back(K.w[i]); S.v.push_back(K.v[i]); } return S;
}
// ---------------- vertex cover
struct VC { int n; std::vector<std::vector<int>> adj; long long nodes = 0; };
bool vc_edge(VC& G, std::vector<char>& del, int k) {                 // pick any remaining edge (u,v): u or v in the cover
    ++G.nodes;
    for (int u = 0; u < G.n; ++u) if (!del[u]) for (int v : G.adj[u]) if (!del[v]) {
        if (k == 0) return false;
        del[u] = 1; bool ok = vc_edge(G, del, k - 1); del[u] = 0; if (ok) return true;
        del[v] = 1; ok = vc_edge(G, del, k - 1); del[v] = 0; return ok;
    }
    return true;                                                          // no edges left
}
bool vc_deg(VC& G, std::vector<char>& del, int k) {                  // max-degree vertex v: take v, or take all of N(v)
    ++G.nodes; int best = -1, bd = 0;
    for (int u = 0; u < G.n; ++u) if (!del[u]) { int d = 0; for (int v : G.adj[u]) d += !del[v]; if (d > bd) { bd = d; best = u; } }
    if (bd == 0) return true;
    if (k == 0) return false;
    if (bd > k) { del[best] = 1; bool ok = vc_deg(G, del, k - 1); del[best] = 0; return ok; }   // N(v) too big: v is forced
    del[best] = 1; bool ok = vc_deg(G, del, k - 1); del[best] = 0; if (ok) return true;
    std::vector<int> nb; for (int v : G.adj[best]) if (!del[v]) nb.push_back(v);
    for (int v : nb) del[v] = 1;
    ok = vc_deg(G, del, k - (int)nb.size());
    for (int v : nb) del[v] = 0;
    return ok;
}
int main() {
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("Part 1: 0/1 knapsack, W = half the total weight (nodes explored; ms)\n");
    std::printf("%4s %-12s | %14s %10s | %14s %10s | %10s\n", "n", "instances", "backtrack nodes", "ms", "B&B nodes", "ms", "DP ms");
    for (bool corr : {false, true}) for (int n : {20, 30, 40, 60, 100}) {
        Knap A = make(n, corr), B = A; ll r1 = -1; double t1 = -1;
        if (n <= 30) t1 = ms([&] { backtrack(A, 0, 0, 0); r1 = A.best; });
        double t2 = ms([&] { bnb(B, 0, 0, 0); }); ll r3 = 0; double t3 = ms([&] { r3 = dp(B); });
        bool ok = B.best == r3 && (r1 < 0 || r1 == r3);
        if (t1 >= 0) std::printf("%4d %-12s | %14lld %10.1f | %14lld %10.1f | %10.1f %s\n", n, corr ? "correlated" : "uncorrelated", A.nodes, t1, B.nodes, t2, t3, ok ? "" : "MISMATCH");
        else std::printf("%4d %-12s | %14s %10s | %14lld %10.1f | %10.1f %s\n", n, corr ? "correlated" : "uncorrelated", "(skipped)", "", B.nodes, t2, t3, ok ? "" : "MISMATCH");
    }
    std::printf("\nPart 2: MINIMUM vertex cover by iterative deepening (k = 0, 1, 2, ... until a cover exists);\n"
                "        random graphs with n vertices and 2n edges; total search nodes over all k\n");
    std::printf("%4s %6s | %16s %10s | %16s %10s\n", "n", "OPT", "edge-branching", "ms", "degree-branching", "ms");
    for (int n : {30, 40, 50, 60}) {
        VC G{n, std::vector<std::vector<int>>(n)};
        for (int e = 0; e < 2 * n; ++e) { int u = rng() % n, v = rng() % n; if (u == v) { --e; continue; } G.adj[u].push_back(v); G.adj[v].push_back(u); }
        VC H = G; std::vector<char> del(n, 0); int k1 = -1, k2 = -1;
        double t1 = (n <= 50) ? ms([&] { for (int k = 0;; ++k) if (vc_edge(G, del, k)) { k1 = k; break; } }) : -1;
        double t2 = ms([&] { for (int k = 0;; ++k) if (vc_deg(H, del, k)) { k2 = k; break; } });
        if (t1 >= 0) std::printf("%4d %6d | %16lld %10.1f | %16lld %10.1f %s\n", n, k2, G.nodes, t1, H.nodes, t2, k1 == k2 ? "" : "MISMATCH");
        else std::printf("%4d %6d | %16s %10s | %16lld %10.1f\n", n, k2, "(skipped)", "", H.nodes, t2);
    }
}
