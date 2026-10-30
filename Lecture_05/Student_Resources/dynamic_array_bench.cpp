// CS612 L05 — Dynamic-array growth policies: amortized cost vs. single-operation worst case vs. wasted memory.
// Build: g++ -std=c++20 -O2 dynamic_array_bench.cpp -o dynarr     Run: ./dynarr   (~15 s)
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
struct DynArray {                      // grows by factor r (multiplicative) or by +c (additive, if r == 0)
    std::uint64_t* a = nullptr; size_t size = 0, cap = 0; double r; size_t c; long long copies = 0;
    DynArray(double r_, size_t c_ = 0) : r(r_), c(c_) {}
    ~DynArray() { std::free(a); }
    void push(std::uint64_t x) {
        if (size == cap) {
            size_t nc = r > 0 ? std::max<size_t>(cap + 1, (size_t)(cap * r)) : cap + c;
            auto* b = (std::uint64_t*)std::malloc(nc * sizeof(std::uint64_t));
            if (size) std::memcpy(b, a, size * sizeof(std::uint64_t));   // the expensive part: copy all items
            copies += size; std::free(a); a = b; cap = nc;
        }
        a[size++] = x;
    }
};
int main() {
    const size_t n = 10'000'000;
    std::printf("n = %zu appends of 8-byte items\n", n);
    std::printf("%-14s %14s %14s %16s %14s %14s\n", "policy", "copies / n", "final cap / n", "total ms", "max op (us)", "p99.99 (ns)");
    struct P { const char* name; double r; size_t c; };
    for (P p : {P{"factor 1.25", 1.25, 0}, P{"factor 1.5", 1.5, 0}, P{"factor 2", 2.0, 0}, P{"factor 3", 3.0, 0}, P{"additive +4096", 0, 4096}}) {
        if (p.r == 0) {                                 // additive growth is quadratic: run on a smaller n
            const size_t m = 300'000; DynArray d(p.r, p.c);
            auto t0 = std::chrono::steady_clock::now(); for (size_t i = 0; i < m; ++i) d.push(i);
            double tot = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
            std::printf("%-14s %14.1f %14.2f %16.1f %14s %14s   (n = %zu only: quadratic)\n", p.name, (double)d.copies / m,
                        (double)d.cap / m, tot, "-", "-", m);
            continue;
        }
        DynArray d(p.r, p.c); std::vector<float> lat(n);
        auto t0 = std::chrono::steady_clock::now();
        for (size_t i = 0; i < n; ++i) {
            auto s = std::chrono::steady_clock::now(); d.push(i);
            lat[i] = (float)std::chrono::duration<double, std::nano>(std::chrono::steady_clock::now() - s).count();
        }
        double tot = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
        float mx = *std::max_element(lat.begin(), lat.end());
        std::nth_element(lat.begin(), lat.begin() + (size_t)(n * 0.9999), lat.end());
        std::printf("%-14s %14.2f %14.2f %16.1f %14.1f %14.0f\n", p.name, (double)d.copies / n, (double)d.cap / n, tot,
                    mx / 1000.0, lat[(size_t)(n * 0.9999)]);
    }
    std::printf("(total ms includes ~2 clock reads per append)\n");
}
