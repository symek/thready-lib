//
// Created by symek on 4/8/25.
//
#pragma once

#include <functional>
#include "ThreadPoolBlocking.hpp"
#include "ThreadPoolSpinning.hpp"
#include "ThreadPoolHybrid.hpp"
#include "RingBufferQueue.hpp"
#include "MutexQueue.hpp"
#include "LockFreeQueue.hpp"

namespace thready {
    using Task = std::function<void()>;
    using BlockingPool = ThreadPoolBlocking <MutexQueue<Task>>;
    using SpinningPool = ThreadPoolSpinning<RingBufferQueue < Task>>;
    using LockFreePool = ThreadPoolSpinning<LockFreeQueue < Task>>;
    using HybridPool = ThreadPoolHybrid<RingBufferQueue<Task>>;
}
