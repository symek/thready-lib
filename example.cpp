//
// Created by symek on 4/8/25.
//
#include <thready/ThreadPools.hpp>
#include <iostream>

int main() {

    const auto thread_count = 5;// std::thread::hardware_concurrency();
    const auto tasks_count = 100;
    const auto miliseconds = 5000;

    std::cout << "Thread count: " << thread_count << std::endl;
    std::cout << "Tasks count: " << tasks_count << std::endl;


    // Test the ThreadPoolBlocking with MutexQueue
    using Bucket = std::array<float, 10>;
    using Raster = std::vector<Bucket>;
    auto raster = Raster(100, Bucket{3.14, 2.71, 1.41, 1.73, 0.577, 1.618, 2.236, 0.707, 1.414, 0.618});


    float multiplier = 1.3;

    auto job = [](Bucket &bucket, float multiplier) {
        for (auto &elem: bucket) {
            elem *= multiplier;
        }
    };


    // Test the ThreadPoolSpinning with RingBufferQueue
    thready::SpinningPool spinning_pool(thread_count, tasks_count);

    for (int i = 0; i < tasks_count; ++i) {
        spinning_pool.enqueue([&raster, multiplier, i, &job]() {
            auto &backet = raster[i];
            job(backet, multiplier);
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds)); // Simulate work
    for (const auto &bucket: raster) {
        for (const auto &elem: bucket) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    thready::LockFreePool lockfree_pool(thread_count, tasks_count);


    return 0;

}