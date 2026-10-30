// CS612 L10 — Shortest paths chosen by assumption. Build: g++ -std=c++20 -O2 sp_bench.cpp -o sp    Run: ./sp  (~40 s)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <queue>
#include <random>
#include <vector>
using ll = long long; const ll INF = (ll)4e18;
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }
struct Arc { int to; ll w; };
using G = std::vector<std::vector<Arc>>;

std::vector<ll> bfs(const G& g, int s) {                               // ignores weights: unit lengths
    std::vector<ll> d(g.size(), INF); std::deque<int> q{s}; d[s] = 0;
    while (!q.empty()) { int u = q.front(); q.pop_front(); for (auto& a : g[u]) if (d[a.to] == INF) { d[a.to] = d[u] + 1; q.push_back(a.to); } }
    return d;
}
std::vector<ll> dijkstra_heap(const G& g, int s) {                     // lazy binary heap: O((n+m) log n)
    std::vector<ll> d(g.size(), INF); d[s] = 0;
    std::priority_queue<std::pair<ll, int>, std::vector<std::pair<ll, int>>, std::greater<>> pq; pq.push({0, s});
    while (!pq.empty()) { auto [du, u] = pq.top(); pq.pop(); if (du > d[u]) continue;
        for (auto& a : g[u]) if (du + a.w < d[a.to]) { d[a.to] = du + a.w; pq.push({d[a.to], a.to}); } }
    return d;
}
std::vector<ll> dijkstra_array(const G& g, int s) {                    // O(n^2 + m)
    int n = g.size(); std::vector<ll> d(n, INF); std::vector<char> done(n, 0); d[s] = 0;
    for (int it = 0; it < n; ++it) { int u = -1; for (int v = 0; v < n; ++v) if (!done[v] && (u < 0 || d[v] < d[u])) u = v;
        if (d[u] == INF) break;
        done[u] = 1;
        for (auto& a : g[u]) if (d[u] + a.w < d[a.to]) d[a.to] = d[u] + a.w;
    }
    return d;
}
std::vector<ll> dial(const G& g, int s, int C) {                       // integer weights in [0,C]: O(m + nC) buckets
    std::vector<ll> d(g.size(), INF); d[s] = 0;
    std::vector<std::vector<int>> B(C + 1); B[0].push_back(s); size_t pending = 1;
    for (ll cur = 0; pending; ++cur) { auto& b = B[cur % (C + 1)];
        while (!b.empty()) { int u = b.back(); b.pop_back(); --pending; if (d[u] != cur) continue;
            for (auto& a : g[u]) if (cur + a.w < d[a.to]) { d[a.to] = cur + a.w; B[d[a.to] % (C + 1)].push_back(a.to); ++pending; } } }
    return d;
}
std::vector<ll> bellman_ford(const G& g, int s, int* rounds) {         // with early termination; O(nm) worst case
    int n = g.size(); std::vector<ll> d(n, INF); d[s] = 0; *rounds = 0;
    for (int r = 0; r < n; ++r) { bool ch = false; ++*rounds;
        for (int u = 0; u < n; ++u) if (d[u] != INF) for (auto& a : g[u]) if (d[u] + a.w < d[a.to]) { d[a.to] = d[u] + a.w; ch = true; }
        if (!ch) break; }
    return d;
}
G random_graph(int n, int deg, ll lo, ll hi) {
    G g(n); std::uniform_int_distribution<int> V(0, n - 1); std::uniform_int_distribution<ll> W(lo, hi);
    for (int u = 0; u < n; ++u) { g[u].push_back({(u + 1) % n, W(rng)}); for (int k = 1; k < deg; ++k) g[u].push_back({V(rng), W(rng)}); }
    return g;
}
int main() {
    { const int n = 200000; G g = random_graph(n, 5, 0, 100);
      std::printf("Part 1: sparse random digraph n = %d, m = %d, integer weights in [0,100]; single source (ms)\n", n, 5 * n);
      std::vector<ll> a, b, c; int rounds = 0;
      double t0 = ms([&] { bfs(g, 0); }), t1 = ms([&] { a = dijkstra_heap(g, 0); }), t2 = ms([&] { b = dial(g, 0, 100); }), t3 = ms([&] { c = bellman_ford(g, 0, &rounds); });
      std::printf("  BFS (wrong answer for weights!) %.1f | Dijkstra heap %.1f | Dial buckets %.1f | Bellman-Ford %.1f (%d rounds, not n = %d)  agree: %s\n",
                  t0, t1, t2, t3, rounds, n, (a == b && b == c) ? "yes" : "NO"); }
    { const int n = 5000;
      std::printf("\nPart 2: Dijkstra, binary heap vs array, n = %d (ms)\n%10s %8s %12s %12s\n", n, "m", "m/n", "heap", "array");
      for (int deg : {5, 50, 500, 4999}) { G g = random_graph(n, deg, 1, 1000000);
          std::vector<ll> a, b; double t1 = ms([&] { a = dijkstra_heap(g, 0); }), t2 = ms([&] { b = dijkstra_array(g, 0); });
          std::printf("%10d %8d %12.1f %12.1f  %s\n", n * deg, deg, t1, t2, a == b ? "" : "MISMATCH"); } }
    { const int n = 1200;
      std::printf("\nPart 3: all pairs with negative edges (no negative cycle), n = %d (ms)\n%8s %14s %16s\n", n, "m/n", "Johnson", "Floyd-Warshall");
      for (int deg : {4, 40, 400}) {
          G g = random_graph(n, deg, 0, 1000); std::uniform_int_distribution<ll> H(0, 2000); std::vector<ll> h(n); for (auto& x : h) x = H(rng);
          for (int u = 0; u < n; ++u) for (auto& a : g[u]) a.w += h[u] - h[a.to];            // reweighting by a random potential: some edges negative, no negative cycles
          std::vector<std::vector<ll>> J(n), F;
          double tj = ms([&] {
              G g2 = g; g2.push_back({}); for (int v = 0; v < n; ++v) g2[n].push_back({v, 0});   // virtual source
              int r; auto p = bellman_ford(g2, n, &r); p.pop_back();
              G gr = g; for (int u = 0; u < n; ++u) for (auto& a : gr[u]) a.w += p[u] - p[a.to];   // non-negative now
              for (int s = 0; s < n; ++s) { auto d = dijkstra_heap(gr, s); for (int v = 0; v < n; ++v) if (d[v] != INF) d[v] += p[v] - p[s]; J[s] = std::move(d); } });
          double tf = ms([&] {
              F.assign(n, std::vector<ll>(n, INF)); for (int u = 0; u < n; ++u) { F[u][u] = 0; for (auto& a : g[u]) F[u][a.to] = std::min(F[u][a.to], a.w); }
              for (int k = 0; k < n; ++k) for (int i = 0; i < n; ++i) { if (F[i][k] == INF) continue; ll dik = F[i][k];
                  for (int j = 0; j < n; ++j) if (F[k][j] != INF && dik + F[k][j] < F[i][j]) F[i][j] = dik + F[k][j]; } });
          std::printf("%8d %14.1f %16.1f  %s\n", deg, tj, tf, J == F ? "" : "MISMATCH"); } }
}
