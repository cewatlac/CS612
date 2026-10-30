// CS612 L09 — (1) Kruskal vs. Prim (binary heap) vs. Prim (array, O(n^2)) across densities;
//             (2) DSU variants on an adversarial linking sequence.
// Build: g++ -std=c++20 -O2 mst_dsu_bench.cpp -o mst     Run: ./mst   (a few minutes: naive linking is slow by design)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <queue>
#include <random>
#include <vector>
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }
struct E { int u, v; std::uint32_t w; };

struct DSU {                       // union by size + path compression (each optional, for Part 2)
    std::vector<int> p, sz; bool bySize, compress; long long steps = 0;
    DSU(int n, bool s = true, bool c = true) : p(n), sz(n, 1), bySize(s), compress(c) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) {
        int r = x; while (p[r] != r) { r = p[r]; ++steps; }
        if (compress) while (p[x] != r) { int nx = p[x]; p[x] = r; x = nx; }
        return r;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b); if (a == b) return false;
        if (bySize && sz[a] > sz[b]) std::swap(a, b);    // attach smaller under larger
        p[a] = b; sz[b] += sz[a]; return true;
    }
};
std::uint64_t kruskal(int n, std::vector<E> e) {
    std::sort(e.begin(), e.end(), [](const E& a, const E& b) { return a.w < b.w; });
    DSU d(n); std::uint64_t tot = 0; int used = 0;
    for (auto& x : e) if (d.unite(x.u, x.v)) { tot += x.w; if (++used == n - 1) break; }
    return tot;
}
std::uint64_t prim_heap(int n, const std::vector<std::vector<std::pair<int, std::uint32_t>>>& adj) {   // lazy binary heap
    std::vector<char> in(n, 0); std::uint64_t tot = 0;
    std::priority_queue<std::pair<std::uint32_t, int>, std::vector<std::pair<std::uint32_t, int>>, std::greater<>> pq;
    pq.push({0, 0});
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop(); if (in[u]) continue;
        in[u] = 1; tot += w;
        for (auto [v, c] : adj[u]) if (!in[v]) pq.push({c, v});
    }
    return tot;
}
std::uint64_t prim_array(int n, const std::vector<std::uint32_t>& M) {           // adjacency matrix, O(n^2)
    const std::uint32_t INF = UINT32_MAX;
    std::vector<std::uint32_t> key(n, INF); std::vector<char> in(n, 0); key[0] = 0; std::uint64_t tot = 0;
    for (int it = 0; it < n; ++it) {
        int u = -1; for (int v = 0; v < n; ++v) if (!in[v] && (u < 0 || key[v] < key[u])) u = v;
        in[u] = 1; tot += key[u];
        for (int v = 0; v < n; ++v) if (!in[v] && M[(size_t)u * n + v] < key[v]) key[v] = M[(size_t)u * n + v];
    }
    return tot;
}
int main() {
    std::setvbuf(stdout, nullptr, _IONBF, 0);            // print progress immediately
    const int n = 4000;
    std::printf("Part 1: MST on random connected graphs, n = %d, weights uniform 32-bit (ms)\n", n);
    std::printf("%12s %10s %10s %12s %12s\n", "m", "m/n", "Kruskal", "Prim(heap)", "Prim(array)");
    std::uniform_int_distribution<std::uint32_t> W(1, 1u << 30);
    for (double dens : {0.001, 0.01, 0.1, 1.0}) {
        const std::uint32_t INF = UINT32_MAX;
        std::vector<std::uint32_t> M((size_t)n * n, INF);
        std::vector<E> e; std::vector<std::vector<std::pair<int, std::uint32_t>>> adj(n);
        auto add = [&](int u, int v) { if (u == v || M[(size_t)u * n + v] != INF) return; std::uint32_t w = W(rng);
            M[(size_t)u * n + v] = M[(size_t)v * n + u] = w; e.push_back({u, v, w}); adj[u].push_back({v, w}); adj[v].push_back({u, w}); };
        for (int i = 1; i < n; ++i) add(i, (int)(rng() % i));                  // random spanning tree: connected
        std::bernoulli_distribution keep(dens);
        for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) if (keep(rng)) add(u, v);
        std::uint64_t a = 0, b = 0, c = 0;
        double t1 = ms([&] { a = kruskal(n, e); }), t2 = ms([&] { b = prim_heap(n, adj); }), t3 = ms([&] { c = prim_array(n, M); });
        if (a != b || b != c) { std::printf("MISMATCH\n"); return 1; }
        std::printf("%12zu %10.1f %10.1f %12.1f %12.1f\n", e.size(), (double)e.size() / n, t1, t2, t3);
    }
    std::printf("\nPart 2a: DSU, adversarial path: unite(i, i+1) for all i, then n finds of element 0 (pointer steps per find)\n");
    std::printf("%9s | %10s %12s %14s %14s\n", "n", "naive", "size only", "compress only", "size+compress");
    for (int N : {10000, 40000, 160000}) {
        double r[4];
        for (int k = 0; k < 4; ++k) { DSU d(N, k & 1, k & 2);
            for (int i = 0; i + 1 < N; ++i) d.unite(i, i + 1);
            d.steps = 0; for (int i = 0; i < N; ++i) d.find(0); r[k] = (double)d.steps / N; }
        std::printf("%9d | %10.1f %12.1f %14.1f %14.1f\n", N, r[0], r[1], r[2], r[3]);
    }
    std::printf("\nPart 2b: DSU, random workload: n elements, n random unions interleaved with 4n random finds\n");
    std::printf("%9s | %10s %12s %14s %14s   (pointer steps per operation; ms for size+compress)\n", "n", "naive", "size only", "compress only", "size+compress");
    for (int N : {100000, 1000000}) {
        double r[4] = {-1, -1, -1, -1}; double tm = 0;
        for (int k = 0; k < 4; ++k) {
            if (k == 0 && N > 100000) continue;          // naive linking builds very deep trees: too slow beyond 1e5
            DSU d(N, k & 1, k & 2); std::mt19937_64 g(7); std::uniform_int_distribution<int> U(0, N - 1);
            double t = ms([&] { for (int i = 0; i < N; ++i) { d.unite(U(g), U(g)); for (int j = 0; j < 4; ++j) d.find(U(g)); } });
            if (k == 3) tm = t;
            r[k] = (double)d.steps / (5.0 * N);
        }
        std::printf("%9d | %10.2f %12.2f %14.2f %14.2f   (%.0f ms)\n", N, r[0], r[1], r[2], r[3], tm);
    }
}
