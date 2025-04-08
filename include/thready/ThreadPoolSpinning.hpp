//
// Created by symek on 4/8/25.
//

#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include "ThreadPoolBase.hpp"

namespace thready {

    template<typename Queue, typename TaskType = std::function<void()>>
    class ThreadPoolSpinning : public ThreadPoolBase<ThreadPoolSpinning<Queue, TaskType>, TaskType> {
    public:
        ThreadPoolSpinning(size_t thread_count, Queue queue)
            : stop_flag(false), tasks(std::move(queue)) {
            for (size_t i = 0; i < thread_count; ++i) {
                workers.emplace_back([this]() {
                    while (!stop_flag.load() || !tasks.empty()) {
                        TaskType task;
                        if (tasks.pop(task)) {
                            task();
                        } else {
                            std::this_thread::yield();
                        }
                    }
                });
            }
        }

        ~ThreadPoolSpinning() {
            stop_flag.store(true);
            for (auto& t : workers) {
                if (t.joinable()) t.join();
            }
        }

        template<typename Func>
        bool enqueue(Func&& f) {
            return tasks.push(TaskType(std::forward<Func>(f)));
        }

        bool has_work() {
            return !tasks.empty();
        }

    private:
        std::vector<std::thread> workers;
        std::atomic<bool> stop_flag;
        Queue tasks;
    };
}

