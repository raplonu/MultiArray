#include <benchmark/benchmark.h>

#include <iostream>
#include <ma/range/Range.h>

using namespace benchmark;

using namespace ma::range;

static void ma_RangeLegacy(State& state) {
    RangeT<RangeLegacy> range(0,100);
    for (auto _ : state)
    {
        auto res = range.select(LinearRange(0,100,2));
        DoNotOptimize(res);
    }

}
BENCHMARK(ma_RangeLegacy);

static void ma_RangeVariant(State& state) {
    RangeT<RangeVariant> range(0,100);
    for (auto _ : state){}
    {
        auto res = range.select(LinearRange(0,100,2));
        DoNotOptimize(res);
    }
}
BENCHMARK(ma_RangeVariant);