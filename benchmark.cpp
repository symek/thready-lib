//
// Created by symek on 4/10/25.

#include <benchmark/benchmark.h>
#include <thready/ThreadPools.hpp>
#include <array>
#include <vector>

using Bucket = std::array<float, 10>;
using Raster = std::vector<Bucket>;

static void BenchmarkSpinningPool(benchmark::State& state) {
    const auto thread_count = state.range(0);
    const auto tasks_count = state.range(1);
    Raster raster(tasks_count, Bucket{3.14, 2.71, 1.41, 1.73, 0.577, 1.618, 2.236, 0.707, 1.414, 0.618});
    float multiplier = 1.3;

    auto job = [](Bucket& bucket, float multiplier) {
        for (auto& elem : bucket) {
            elem *= multiplier;
        }
    };

    for (auto _ : state) {
        thready::SpinningPool spinning_pool(thread_count, tasks_count);
        for (int i = 0; i < tasks_count; ++i) {
            spinning_pool.enqueue([&raster, multiplier, i, job]() {
                auto& bucket = raster[i];
                job(bucket, multiplier);
            });
        }
        spinning_pool.wait_until_empty();
    }
}

static void BenchmarkLockFreePool(benchmark::State& state) {
    const auto thread_count = state.range(0);
    const auto tasks_count = state.range(1);
//    Raster raster(tasks_count, Bucket{3.14, 2.71, 1.41, 1.73, 0.577, 1.618, 2.236, 0.707, 1.414, 0.618});
//    float multiplier = 1.3;

    auto job = [](Bucket& bucket, float multiplier) {
        for (auto& elem : bucket) {
            elem *= multiplier;
        }
    };

    for (auto _ : state) {
        Raster raster(tasks_count, Bucket{3.14, 2.71, 1.41, 1.73, 0.577, 1.618, 2.236, 0.707, 1.414, 0.618});
        const float multiplier = 1.3;
        thready::LockFreePool lockfree_pool(thread_count, tasks_count);
        for (auto & bucket : raster) {
            lockfree_pool.enqueue([multiplier, job, &bucket]() {
                job(bucket, multiplier);
            });
        }
        lockfree_pool.wait_until_empty();
    }
}

static void BenchmarkHybridPool(benchmark::State& state) {
    const auto thread_count = state.range(0);
    const auto tasks_count = state.range(1);
    Raster raster(tasks_count, Bucket{3.14, 2.71, 1.41, 1.73, 0.577, 1.618, 2.236, 0.707, 1.414, 0.618});
    float multiplier = 1.3;

    auto job = [](Bucket& bucket, float multiplier) {
        for (auto& elem : bucket) {
            elem *= multiplier;
        }
    };

    for (auto _ : state) {
        thready::HybridPool hybrid_pool(thread_count, tasks_count);
        for (int i = 0; i < tasks_count; ++i) {
            hybrid_pool.enqueue([&raster, multiplier, i, job]() {
                auto& bucket = raster[i];
                job(bucket, multiplier);
            });
        }
        hybrid_pool.wait_until_empty();
    }
}

//BENCHMARK(BenchmarkSpinningPool)->Args({4, 64*4})->Args({8, 128*8});
BENCHMARK(BenchmarkLockFreePool)->Args({2, 4})->Args({2, 8});
//BENCHMARK(BenchmarkHybridPool)->Args({4, 64*4})->Args({8, 128*8});

BENCHMARK_MAIN();


