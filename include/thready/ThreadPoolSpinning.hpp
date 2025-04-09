//
// Created by symek on 4/8/25.
//

#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <type_traits> // for std::is_constructible_v

namespace thready {


    template<typename Queue, typename TaskType = std::function<void()>>
    class ThreadPoolSpinning {
    public:
        ThreadPoolSpinning(size_t thread_count, size_t queue_capacity )
                : stop_flag(false),
                  tasks([&] {
                      if constexpr (std::is_constructible_v<Queue, size_t>) {
                          return Queue(queue_capacity);
                      } else {
                          return Queue();
                      }
                  }()) {
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
            for (auto &t: workers) {
                if (t.joinable()) t.join();
            }
        }

        template<typename Func>
        void enqueue(Func &&f) {
            tasks.push(TaskType(std::forward<Func>(f)));
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

#include "ThreadPoolBase.hpp"

