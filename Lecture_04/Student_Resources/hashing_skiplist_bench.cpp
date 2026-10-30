// CS612 L04 — (1) Fixed vs. randomly chosen hash functions under structured keys; (2) skip list vs. red-black tree vs. hash table.
// Build: g++ -std=c++20 -O2 hashing_skiplist_bench.cpp -o bench     Run: ./bench   (~30 s)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <map>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>
using u64 = std::uint64_t;
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }

// ---------- Part 1: chained hash table with m = 2^16 buckets, two hash functions ----------
constexpr int LG = 16; constexpr u64 M = 1ull << LG;
struct Chained {
    std::vector<std::vector<u64>> b; bool universal; u64 a;
    Chained(bool uni) : b(M), universal(uni), a(rng() | 1) {}              // random odd multiplier, drawn at construction
    size_t h(u64 k) const { return universal ? (size_t)((a * k) >> (64 - LG)) : (size_t)(k % M); }  // multiply-shift vs. k mod m
    void insert(u64 k) { b[h(k)].push_back(k); }
    size_t probes(u64 k) const { const auto& c = b[h(k)]; size_t i = 0; while (i < c.size() && c[i] != k) ++i; return i + 1; }
};
void part1() {
    std::printf("Part 1: chaining, n = m = 65536 keys (load factor 1); 10^6 successful lookups\n");
    std::printf("%-26s | %-9s %8s %10s %9s\n", "key set", "hash", "max chain", "avg probes", "ms");
    const size_t n = M;
    for (int kind = 0; kind < 3; ++kind) {
        std::vector<u64> keys(n);
        const char* name = kind == 0 ? "random 64-bit" : kind == 1 ? "sequential 0..n-1" : "multiples of 65536";
        for (size_t i = 0; i < n; ++i) keys[i] = kind == 0 ? rng() : kind == 1 ? i : i * M;
        for (int uni = 0; uni < 2; ++uni) {
            Chained t(uni); for (auto k : keys) t.insert(k);
            size_t mx = 0; for (auto& c : t.b) mx = std::max(mx, c.size());
            std::uniform_int_distribution<size_t> pick(0, n - 1);
            std::vector<u64> q(1000000); for (auto& x : q) x = keys[pick(rng)];
            double tot = 0; double tm = ms([&] { for (auto k : q) tot += t.probes(k); });
            std::printf("%-26s | %-9s %8zu %10.2f %9.1f\n", name, uni ? "universal" : "k mod m", mx, tot / q.size(), tm);
        }
    }
}
// ---------- Part 2: skip list (p = 1/2) ----------
struct SkipList {
    static constexpr int MAXL = 40;
    struct Node { u64 key; std::vector<Node*> next; Node(u64 k, int l) : key(k), next(l, nullptr) {} };
    Node head{0, MAXL}; int level = 1; long long steps = 0;
    int randomLevel() { int l = 1; while (l < MAXL && (rng() & 1)) ++l; return l; }
    void insert(u64 k) {
        Node* upd[MAXL]; Node* x = &head;
        for (int i = level - 1; i >= 0; --i) { while (x->next[i] && x->next[i]->key < k) x = x->next[i]; upd[i] = x; }
        int l = randomLevel();
        if (l > level) { for (int i = level; i < l; ++i) upd[i] = &head; level = l; }
        Node* nd = new Node(k, l);
        for (int i = 0; i < l; ++i) { nd->next[i] = upd[i]->next[i]; upd[i]->next[i] = nd; }
    }
    bool find(u64 k) {
        Node* x = &head;
        for (int i = level - 1; i >= 0; --i) while (x->next[i] && x->next[i]->key < k) { x = x->next[i]; ++steps; }
        x = x->next[0]; return x && x->key == k;
    }
};
void part2() {
    std::printf("\nPart 2: n = 10^6 distinct keys; insert all, then 10^6 successful lookups (ms)\n");
    std::printf("%-10s | %10s %10s | %10s %10s | %10s %10s | %9s %11s\n", "order", "skip ins", "skip find", "map ins", "map find",
                "umap ins", "umap find", "SL height", "steps/find");
    const size_t n = 1000000;
    for (int sorted = 0; sorted < 2; ++sorted) {
        std::vector<u64> keys(n); std::iota(keys.begin(), keys.end(), 1);
        for (auto& k : keys) k *= 2654435761ull;          // spread values, keep them distinct
        if (sorted) std::sort(keys.begin(), keys.end()); else std::shuffle(keys.begin(), keys.end(), rng);
        std::vector<u64> q = keys; std::shuffle(q.begin(), q.end(), rng);
        SkipList sl; std::map<u64, int> mp; std::unordered_map<u64, int> um; um.reserve(n);
        double a = ms([&] { for (auto k : keys) sl.insert(k); });
        long long hit = 0;
        double b = ms([&] { for (auto k : q) hit += sl.find(k); });
        double c = ms([&] { for (auto k : keys) mp.emplace(k, 0); });
        double d = ms([&] { for (auto k : q) hit += mp.count(k); });
        double e = ms([&] { for (auto k : keys) um.emplace(k, 0); });
        double f = ms([&] { for (auto k : q) hit += um.count(k); });
        if (hit != 3LL * (long long)n) { std::printf("LOOKUP ERROR\n"); return; }
        std::printf("%-10s | %10.1f %10.1f | %10.1f %10.1f | %10.1f %10.1f | %9d %11.1f\n", sorted ? "sorted" : "random",
                    a, b, c, d, e, f, sl.level, (double)sl.steps / n);
    }
    std::printf("(2 log2 n = %.1f)\n", 2 * std::log2((double)n));
}
int main() { part1(); part2(); }
