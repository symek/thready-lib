//
// Created by symek on 4/8/25.
//
#include "thready/ThreadPools.hpp"
#include <iostream>

int main(){
    // Test the ThreadPoolBlocking with MutexQueue
//    thready::BlockingPool blocking_pool(4);
//    blocking_pool.enqueue([] { std::cout << "Blocking Pool Task" << std::endl; });
//    blocking_pool.wait_until_empty();

    // Test the ThreadPoolSpinning with RingBufferQueue
    thready::SpinningPool spinning_pool(4, 12);
    spinning_pool.enqueue([] { std::cout << "Spinning Pool Task" << std::endl; });
//    spinning_pool.wait_until_empty();

    // Test the ThreadPoolHybrid with RingBufferQueue
    thready::HybridPool hybrid_pool(4, 12);
    hybrid_pool.enqueue([] { std::cout << "Hybrid Pool Task" << std::endl; });
//    hybrid_pool.wait_until_empty();

    return 0;
}