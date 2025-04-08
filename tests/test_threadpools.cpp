//
// Created by symek on 4/8/25.
//
#include <catch2/catch_test_macros.hpp>
#include "../include/thready/ThreadPools.hpp"

TEST_CASE("SpinningPool processes tasks", "[threadpool]") {
    thready::RingBufferQueue<thready::Task> q(10);
    thready::SpinningPool pool(2, std::move(q));

    std::atomic<int> counter{0};

    for (int i = 0; i < 5; ++i) {
        pool.enqueue([&counter]() { ++counter; });
    }

    while (counter.load() < 5) {
        std::this_thread::yield();
    }

    REQUIRE(counter.load() == 5);
}
