// CS612 L11 — (1) naive vs. KMP vs. Rabin–Karp (random base, 61-bit prime, every hash hit verified);
//             (2) Aho–Corasick vs. k separate KMP scans.
// Build: g++ -std=c++20 -O2 strings_bench.cpp -o strings     Run: ./strings   (~40 s)
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <random>
#include <string>
#include <vector>
using u64 = std::uint64_t; using u128 = unsigned __int128;
static std::mt19937_64 rng(612);
template <class F> double ms(F f) { auto t = std::chrono::steady_clock::now(); f();
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t).count(); }

long long naive(const std::string& T, const std::string& P) {
    long long c = 0; size_t n = T.size(), m = P.size();
    for (size_t s = 0; s + m <= n; ++s) { size_t j = 0; while (j < m && T[s + j] == P[j]) ++j; c += (j == m); }
    return c;
}
std::vector<int> prefix_function(const std::string& P) {
    std::vector<int> pi(P.size(), 0);
    for (size_t i = 1, k = 0; i < P.size(); ++i) { while (k && P[i] != P[k]) k = pi[k - 1]; if (P[i] == P[k]) ++k; pi[i] = (int)k; }
    return pi;
}
long long kmp(const std::string& T, const std::string& P, const std::vector<int>& pi) {
    long long c = 0; size_t q = 0, m = P.size();
    for (char ch : T) { while (q && ch != P[q]) q = pi[q - 1]; if (ch == P[q]) ++q; if (q == m) { ++c; q = pi[q - 1]; } }
    return c;
}
const u64 MOD = (1ull << 61) - 1;                       // Mersenne prime
u64 mulmod(u64 a, u64 b) { u128 z = (u128)a * b; u64 r = (u64)(z & MOD) + (u64)(z >> 61); return r >= MOD ? r - MOD : r; }
long long rabin_karp(const std::string& T, const std::string& P, long long* false_hits) {
    std::uniform_int_distribution<u64> B(256, MOD - 1); u64 b = B(rng);   // base chosen at random AFTER the input is fixed
    size_t n = T.size(), m = P.size(); if (m > n) return 0;
    u64 hp = 0, ht = 0, bm = 1;
    for (size_t i = 0; i < m; ++i) { hp = (mulmod(hp, b) + (unsigned char)P[i]) % MOD; ht = (mulmod(ht, b) + (unsigned char)T[i]) % MOD; if (i) bm = mulmod(bm, b); }
    long long c = 0; *false_hits = 0;
    for (size_t s = 0;; ++s) {
        if (ht == hp) { if (T.compare(s, m, P) == 0) ++c; else ++*false_hits; }        // verify: Las Vegas
        if (s + m >= n) break;
        ht = (ht + MOD - mulmod(bm, (unsigned char)T[s])) % MOD;
        ht = (mulmod(ht, b) + (unsigned char)T[s + m]) % MOD;
    }
    return c;
}
struct AhoCorasick {                                    // alphabet 'a'..'z'; full goto automaton
    std::vector<std::array<int, 26>> go; std::vector<int> fail, out;    // out = number of patterns ending here (incl. via suffix links)
    AhoCorasick() { go.push_back({}); go[0].fill(-1); out.push_back(0); }
    void add(const std::string& s) { int v = 0; for (char ch : s) { int c = ch - 'a'; if (go[v][c] < 0) { go[v][c] = (int)go.size(); go.push_back({}); go.back().fill(-1); out.push_back(0); } v = go[v][c]; } ++out[v]; }
    void build() { fail.assign(go.size(), 0); std::queue<int> q;
        for (int c = 0; c < 26; ++c) { if (go[0][c] < 0) go[0][c] = 0; else q.push(go[0][c]); }
        while (!q.empty()) { int v = q.front(); q.pop(); out[v] += out[fail[v]];
            for (int c = 0; c < 26; ++c) { int u = go[v][c]; if (u < 0) go[v][c] = go[fail[v]][c]; else { fail[u] = go[fail[v]][c]; q.push(u); } } } }
    long long count(const std::string& T) const { long long c = 0; int v = 0; for (char ch : T) { v = go[v][ch - 'a']; c += out[v]; } return c; }
};
int main() {
    std::printf("Part 1: single pattern, text n = 3e7 (ms)\n%-34s %9s %9s %9s %9s %12s\n", "text / pattern", "naive", "KMP", "RK", "matches", "RK false hits");
    const size_t n = 30'000'000, m = 1000;
    for (int kind = 0; kind < 3; ++kind) {
        std::string T(n, 'a'), P(m, 'a');
        const char* name;
        if (kind == 0) { name = "random {a..z} / random"; for (auto& ch : T) ch = 'a' + rng() % 26; for (auto& ch : P) ch = 'a' + rng() % 26; }
        else if (kind == 1) { name = "random {a,b} / random"; for (auto& ch : T) ch = 'a' + rng() % 2; for (auto& ch : P) ch = 'a' + rng() % 2; }
        else { name = "a^n / a^(m-1)b  (adversarial)"; P[m - 1] = 'b'; }
        long long a = 0, b = 0, c = 0, fh = 0; auto pi = prefix_function(P);
        double t0 = ms([&] { a = naive(T, P); }), t1 = ms([&] { b = kmp(T, P, pi); }), t2 = ms([&] { c = rabin_karp(T, P, &fh); });
        std::printf("%-34s %9.0f %9.0f %9.0f %9lld %12lld %s\n", name, t0, t1, t2, b, fh, (a == b && b == c) ? "" : "MISMATCH");
    }
    std::printf("\nPart 2: k patterns of length 8 (sampled from the text), text n = 1e7 over {a..z} (ms)\n%8s %14s %14s %12s %10s\n", "k", "k x KMP", "Aho-Corasick", "AC states", "matches");
    std::string T(10'000'000, 'a'); for (auto& ch : T) ch = 'a' + rng() % 26;
    for (int k : {1, 10, 100, 1000, 10000}) {
        std::vector<std::string> pats; for (int i = 0; i < k; ++i) pats.push_back(T.substr(rng() % (T.size() - 8), 8));
        long long c1 = 0, c2 = 0; AhoCorasick ac;
        double t1 = (k <= 1000) ? ms([&] { for (auto& p : pats) c1 += kmp(T, p, prefix_function(p)); }) : -1;
        double t2 = ms([&] { for (auto& p : pats) ac.add(p); ac.build(); c2 = ac.count(T); });
        if (t1 >= 0) std::printf("%8d %14.0f %14.0f %12zu %10lld %s\n", k, t1, t2, ac.go.size(), c2, c1 == c2 ? "" : "MISMATCH");
        else std::printf("%8d %14s %14.0f %12zu %10lld\n", k, "(skipped)", t2, ac.go.size(), c2);
    }
}
