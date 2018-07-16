#include <benchmark/benchmark.h>

#include <iostream>
#include <ma/iterator/Iterator.h>

using namespace benchmark;

using namespace ma::iterator;

static void ma_IteratorLegacy(State& state) {
    IteratorT<IteratorLegacy> it;
    for (auto _ : state)
        DoNotOptimize(++it);

}
BENCHMARK(ma_IteratorLegacy);

static void ma_IteratorVariant(State& state) {
    IteratorT<IteratorVariant> it;
    for (auto _ : state){}
        DoNotOptimize(++it);
}
BENCHMARK(ma_IteratorVariant);