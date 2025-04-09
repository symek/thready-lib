//
// Created by symek on 4/8/25.
//
#include <catch2/catch_test_macros.hpp>
#include "../include/thready/ThreadPools.hpp"

TEST_CASE("SpinningPool processes tasks", "[threadpool]") {
//    thready::RingBufferQueue<thready::Task> q(10);
    thready::SpinningPool pool(2, 10);

    std::atomic<int> counter{0};

    for (int i = 0; i < 5; ++i) {
        pool.enqueue([&counter]() { ++counter; });
    }

    while (counter.load() < 5) {
        std::this_thread::yield();
    }

    REQUIRE(counter.load() == 5);
}

#include <catch2/catch_test_macros.hpp>
#include "../include/thready/ThreadPools.hpp"
#include <atomic>
#include <chrono>
#include <thread>

using namespace thready;
using namespace std::chrono_literals;

template<typename Pool>
void test_basic_execution(Pool& pool, int task_count) {
    std::atomic<int> counter{0};

    for (int i = 0; i < task_count; ++i) {
        pool.enqueue([&counter]() {
            std::this_thread::sleep_for(1ms);
            ++counter;
        });
    }

    pool.wait_until_empty();
    REQUIRE(counter == task_count);
}

template<typename Pool>
void test_saturation(Pool& pool, int total, int capacity) {
    std::atomic<int> counter{0};

    int enqueued = 0;
    for (int i = 0; i < total; ++i) {
        pool.enqueue([&counter]() {
            std::this_thread::sleep_for(1ms);
            ++counter;
        });

            ++enqueued;

    }

    // Some queues may drop tasks if full
    REQUIRE(enqueued <= capacity);
    pool.wait_until_empty();
    REQUIRE(counter == enqueued);
}

// ----------------------------
// Test cases
// ----------------------------

//TEST_CASE("BlockingPool executes tasks") {
//    BlockingPool pool(4);
//    test_basic_execution(pool, 20);
//}

TEST_CASE("SpinningPool executes tasks") {
    SpinningPool pool(4, 64);
    test_basic_execution(pool, 32);
}

TEST_CASE("HybridPool executes tasks") {
    HybridPool pool(4, 64);
    test_basic_execution(pool, 32);
}

TEST_CASE("HybridPool respects queue capacity") {
    HybridPool pool(2, 8); // Only 8 tasks fit in queue
    test_saturation(pool, 100, 8);
}

TEST_CASE("SpinningPool respects queue capacity") {
    SpinningPool pool(2, 8); // RingBufferQueue
    test_saturation(pool, 100, 8);
}

