//
// Created by symek on 4/8/25.
//
#include "thready/RingBufferQueue.hpp"
#include "thready/MutexQueue.hpp"
#include "thready/LockFreeQueue.hpp"
#include "thready/ThreadPoolBlocking.hpp"
#include "thready/ThreadPoolSpinning.hpp"
#include "thready/ThreadPoolHybrid.hpp"

namespace thready {
    // Define what Task is
    using Task = std::function<void()>;
    // Explicitly instantiate all pool types
    template class ThreadPoolSpinning<LockFreeQueue<thready::Task>>;
    template class ThreadPoolBlocking<MutexQueue<Task>>;
    template class ThreadPoolSpinning<RingBufferQueue<Task>>;
    template class ThreadPoolHybrid<RingBufferQueue<Task>>;

}
