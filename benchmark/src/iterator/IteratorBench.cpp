#include <benchmark/benchmark.h>

#include <ma_api/iterator/Iterator.h>

using namespace benchmark;

using namespace ma::iterator;

static void ma_IteratorLegacy(State& state) {
    IteratorT<IteratorLegacy> it;
    for (auto _ : state)
        for(int i(0); i < 1000; ++i)
            DoNotOptimize(*++it);

}
BENCHMARK(ma_IteratorLegacy);

static void ma_IteratorVariant(State& state) {
    IteratorT<IteratorVariant> it;
    for (auto _ : state)
        for(int i(0); i < 1000; ++i)
            DoNotOptimize(*++it);
}
BENCHMARK(ma_IteratorVariant);