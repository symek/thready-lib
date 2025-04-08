//
// Created by symek on 4/8/25.
//
#include "thready/ThreadPoolBlocking.hpp"
#include "thready/ThreadPoolSpinning.hpp"
#include "thready/ThreadPoolHybrid.hpp"
#include "thready/RingBufferQueue.hpp"
#include "thready/MutexQueue.hpp"
#include "thready/LockFreeQueue.hpp"

namespace thready {
    // Define what Task is
    using Task = std::function<void()>;

    // Explicitly instantiate all pool types
    template class ThreadPoolBlocking<MutexQueue<Task>>;
    template class ThreadPoolSpinning<RingBufferQueue<Task>>;
    template class ThreadPoolSpinning<LockFreeQueue<Task>>;
    template class ThreadPoolHybrid<RingBufferQueue<Task>>;
}
