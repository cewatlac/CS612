// CS612 L12 — Ford–Fulkerson (DFS augmenting paths) vs. Edmonds–Karp (BFS) vs. Dinic.
// Build: g++ -std=c++20 -O2 flow_bench.cpp -o flow     Run: ./flow   (~20 s)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <random>
#include <vector>
using ll = long long;
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }
struct Net {                                   // residual graph with paired arcs (i, i^1)
    struct A { int to; ll cap; };
    std::vector<A> e; std::vector<std::vector<int>> g; long long augmentations = 0;
    explicit Net(int n) : g(n) {}
    void add(int u, int v, ll c) { g[u].push_back(e.size()); e.push_back({v, c}); g[v].push_back(e.size()); e.push_back({u, 0}); }
    // --- Ford–Fulkerson with DFS (iterative), augment along any path found
    ll ff_dfs(int s, int t) {
        ll flow = 0; int n = g.size();
        while (true) {
            std::vector<int> pe(n, -1), st{s}; std::vector<char> seen(n, 0); seen[s] = 1;
            while (!st.empty() && !seen[t]) { int u = st.back(); st.pop_back();
                for (int id : g[u]) if (e[id].cap > 0 && !seen[e[id].to]) { seen[e[id].to] = 1; pe[e[id].to] = id; st.push_back(e[id].to); } }
            if (!seen[t]) return flow;
            ll b = INT64_MAX; for (int v = t; v != s; v = e[pe[v] ^ 1].to) b = std::min(b, e[pe[v]].cap);
            for (int v = t; v != s; v = e[pe[v] ^ 1].to) { e[pe[v]].cap -= b; e[pe[v] ^ 1].cap += b; }
            flow += b; ++augmentations;
        }
    }
    // --- Ford–Fulkerson with an UNLUCKY but legal path rule: always augment along a LONGEST simple
    //     augmenting path (exhaustive search; only for tiny graphs). FF allows any augmenting path.
    std::vector<int> best, cur; std::vector<char> on;
    void longest(int u, int t) { if (u == t) { if (cur.size() > best.size()) best = cur; return; }
        for (int id : g[u]) { int v = e[id].to; if (e[id].cap > 0 && !on[v]) { on[v] = 1; cur.push_back(id); longest(v, t); cur.pop_back(); on[v] = 0; } } }
    ll ff_longest(int s, int t) {
        ll flow = 0;
        while (true) { best.clear(); cur.clear(); on.assign(g.size(), 0); on[s] = 1; longest(s, t);
            if (best.empty()) return flow;
            ll b = INT64_MAX; for (int id : best) b = std::min(b, e[id].cap);
            for (int id : best) { e[id].cap -= b; e[id ^ 1].cap += b; }
            flow += b; ++augmentations; }
    }
    // --- Edmonds–Karp: shortest augmenting paths
    ll edmonds_karp(int s, int t) {
        ll flow = 0; int n = g.size();
        while (true) {
            std::vector<int> pe(n, -1); std::queue<int> q; q.push(s); pe[s] = -2;
            while (!q.empty() && pe[t] == -1) { int u = q.front(); q.pop();
                for (int id : g[u]) if (e[id].cap > 0 && pe[e[id].to] == -1) { pe[e[id].to] = id; q.push(e[id].to); } }
            if (pe[t] == -1) return flow;
            ll b = INT64_MAX; for (int v = t; v != s; v = e[pe[v] ^ 1].to) b = std::min(b, e[pe[v]].cap);
            for (int v = t; v != s; v = e[pe[v] ^ 1].to) { e[pe[v]].cap -= b; e[pe[v] ^ 1].cap += b; }
            flow += b; ++augmentations;
        }
    }
    // --- Dinic: BFS level graph + blocking flow with current-arc pointers
    std::vector<int> lvl, it;
    bool bfs(int s, int t) { lvl.assign(g.size(), -1); std::queue<int> q; q.push(s); lvl[s] = 0;
        while (!q.empty()) { int u = q.front(); q.pop(); for (int id : g[u]) if (e[id].cap > 0 && lvl[e[id].to] < 0) { lvl[e[id].to] = lvl[u] + 1; q.push(e[id].to); } }
        return lvl[t] >= 0; }
    ll dfs(int u, int t, ll f) { if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) { int id = g[u][i]; int v = e[id].to;
            if (e[id].cap > 0 && lvl[v] == lvl[u] + 1) { ll d = dfs(v, t, std::min(f, e[id].cap)); if (d > 0) { e[id].cap -= d; e[id ^ 1].cap += d; return d; } } }
        return 0; }
    ll dinic(int s, int t) { ll flow = 0; while (bfs(s, t)) { it.assign(g.size(), 0); while (ll f = dfs(s, t, INT64_MAX)) { flow += f; ++augmentations; } } return flow; }
};
int main() {
    std::printf("Part 1: the classic bad instance for Ford–Fulkerson (4 nodes, capacities C and one middle edge of capacity 1),\n"
                "        with the legal but unlucky rule 'augment along a longest augmenting path'\n");
    std::printf("%12s | %16s %16s | %16s\n", "C", "FF augm.", "FF ms", "Edmonds-Karp augm.");
    for (ll C : {1000LL, 100000LL, 1000000LL}) {
        auto build = [&] { Net N(4); N.add(0, 2, C); N.add(2, 3, C); N.add(0, 1, C); N.add(1, 3, C); N.add(1, 2, 1);   
            return N; };
        Net a = build(), b = build(); ll f1 = 0; double t = ms([&] { f1 = a.ff_longest(0, 3); }); ll f2 = b.edmonds_karp(0, 3);
        std::printf("%12lld | %16lld %16.1f | %16lld   (max flow %lld = %lld)\n", C, a.augmentations, t, b.augmentations, f1, f2);
    }
    std::printf("\nPart 2: maximum bipartite matching, random bipartite graphs with n + n vertices, average degree d (ms)\n");
    std::printf("%8s %4s | %12s %12s %12s | %10s\n", "n", "d", "FF-DFS", "Edm.-Karp", "Dinic", "matching");
    for (int n : {2000, 20000, 100000}) for (int d : {3, 10}) {
        auto build = [&] { Net N(2 * n + 2); int s = 2 * n, t = 2 * n + 1; std::mt19937_64 r(n * 31 + d);
            for (int i = 0; i < n; ++i) { N.add(s, i, 1); N.add(n + i, t, 1); for (int k = 0; k < d; ++k) N.add(i, n + (int)(r() % n), 1); }
            return N; };
        int s = 2 * n, t = 2 * n + 1; ll a = 0, b = 0, c = 0; double t1 = -1, t2 = -1;
        Net A = build(), B = build(), C3 = build();
        if (n <= 20000) { t1 = ms([&] { a = A.ff_dfs(s, t); }); t2 = ms([&] { b = B.edmonds_karp(s, t); }); }
        double t3 = ms([&] { c = C3.dinic(s, t); });
        if (t1 >= 0) std::printf("%8d %4d | %12.1f %12.1f %12.1f | %10lld %s\n", n, d, t1, t2, t3, c, (a == b && b == c) ? "" : "MISMATCH");
        else std::printf("%8d %4d | %12s %12s %12.1f | %10lld\n", n, d, "(skipped)", "(skipped)", t3, c);
    }
}
