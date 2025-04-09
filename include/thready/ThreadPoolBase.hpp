//
// Created by symek on 4/8/25.
//

#pragma once
#include <thread>
#include <vector>
#include <atomic>
#include <functional>


namespace thready {

    template<typename Derived, typename TaskType = std::function<void()>>
    class ThreadPoolBase {
    public:
        template<typename Func>
         void enqueue(Func&& f) {
             static_cast<Derived*>(this)->push(std::forward<Func>(f));
        }

        bool has_work() {
            return static_cast<Derived*>(this)->has_work();
        }

        void wait_until_empty() {
            while (has_work()) {
                std::this_thread::yield();
            }
        }
    };
}
