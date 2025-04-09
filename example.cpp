//
// Created by symek on 4/8/25.
//
#include <thready/ThreadPools.hpp>
#include <iostream>

int main(){

    const auto thread_count = std::thread::hardware_concurrency();
    const auto tasks_count = 20;
    const auto miliseconds = 1500;

    std::cout << "Thread count: " << thread_count << std::endl;
    std::cout << "Tasks count: " << tasks_count << std::endl;

    // Test the ThreadPoolBlocking with LockFreeQueue
//    thready::BlockingPool blocking_pool(4);
//    for (int i = 0; i < tasks_count; ++i) {
//        blocking_pool.enqueue([i] { std::cout << "Blocking Pool Task " << i << std::endl; });
//    }
//    std::cout << "Blocking Pool finished" << std::endl;

    // Test the ThreadPoolSpinning with RingBufferQueue
    thready::SpinningPool spinning_pool(thread_count, tasks_count);

    for (int i = 0; i < tasks_count; ++i) {
        spinning_pool.enqueue([i, miliseconds] {
            std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds)); // Simulate work});
            std::cout << "Spinning Pool Task " << i << std::endl;
        });
    }
    std::cout << "Spinning Pool finished" << std::endl;
    // Test the ThreadPoolHybrid with RingBufferQueue
    thready::HybridPool hybrid_pool(thread_count, tasks_count);
    for (int i = 0; i < tasks_count; ++i) {
        hybrid_pool.enqueue([i, miliseconds] {
            std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds)); // Simulate work});});
            std::cout << "Hybrid Pool Task " << i << std::endl;
        });
    }
    std::cout << "Hybrid Pool finished" << std::endl;


//     Test the ThreadPoolSpinning with LockFreeQueue
    thready::LockFreePool lockfree_pool(thread_count, tasks_count);
    for (int i = 0; i < tasks_count; ++i) {
        lockfree_pool.enqueue([i] { std::cout << "LockFree Pool Task " << i << std::endl; });
    }
        std::cout << "LockFree Pool finished" << std::endl;


    // Test the ThreadPoolBlocking with MutexQueue
//    thready::BlockingPool blocking_pool(4);
//    blocking_pool.enqueue([] { std::cout << "Blocking Pool Task" << std::endl; });
//    blocking_pool.wait_until_empty();

    // Test the ThreadPoolSpinning with RingBufferQueue
//    thready::SpinningPool spinning_pool(4, 12);

//    spinning_pool.enqueue([] { std::cout << "Spinning Pool Task" << std::endl; });
//    spinning_pool.wait_until_empty();

    // Test the ThreadPoolHybrid with RingBufferQueue
//    thready::HybridPool hybrid_pool(4, 12);
//    hybrid_pool.enqueue([] { std::cout << "Hybrid Pool Task" << std::endl; });
//    hybrid_pool.wait_until_empty();

    return 0;
}