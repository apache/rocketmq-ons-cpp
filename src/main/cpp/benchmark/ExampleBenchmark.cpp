#include <benchmark/benchmark.h>
#include <atomic>

int simpleLoopRelax(int n) {
    std::atomic_long s(0);
    for (int i = 0; i < n; ++i) {
        s.fetch_add(1, std::memory_order_relaxed);
    }
    return s.load(std::memory_order_relaxed);
}

int simpleLoopSeq(int n) {
    std::atomic_long s(0);
    for (int i = 0; i < n; ++i) {
        s.fetch_add(1, std::memory_order_seq_cst);
    }
    return s.load(std::memory_order_seq_cst);
}

static void BM_simpleLoopRelaxed(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(simpleLoopRelax(state.range(0)));
    }
}

static void BM_simpleLoopSeq(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(simpleLoopSeq(state.range(0)));
    }
}

BENCHMARK(BM_simpleLoopRelaxed)->RangeMultiplier(2)->Range(1, 1<<10)->UseRealTime()->Unit(benchmark::kNanosecond);
BENCHMARK(BM_simpleLoopSeq)->RangeMultiplier(2)->Range(1, 1<<10)->UseRealTime()->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();