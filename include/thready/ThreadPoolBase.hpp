//
// Created by symek on 4/8/25.
//
#pragma once

#include <functional>
#include <thread>

namespace thready {

    template<typename Derived, typename TaskType = std::function<void()>>
    class ThreadPoolBase {
    public:
        template<typename Func>
        void enqueue(Func&& f) {
            static_cast<Derived*>(this)->tasks.push(TaskType(std::forward<Func>(f)));
        }

        [[nodiscard]] bool has_work() const {
            return !static_cast<const Derived*>(this)->tasks.empty();
        }

        void wait_until_empty() {
            while (has_work()) {
                std::this_thread::yield();
            }
        }
    };
}

