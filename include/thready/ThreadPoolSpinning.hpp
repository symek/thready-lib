//
// Created by symek on 4/8/25.
//
#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <type_traits>
#include "ThreadPoolBase.hpp"
#include <iostream>
namespace thready {

    template<typename Queue, typename TaskType = std::function<void()>>
    class ThreadPoolSpinning : public ThreadPoolBase<ThreadPoolSpinning<Queue, TaskType>, TaskType> {
    public:
        ThreadPoolSpinning(size_t thread_count, size_t queue_capacity)
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
            // Wait for the queue to become empty
            while (!tasks.empty()) {
                std::this_thread::yield();
            }
            stop_flag.store(true);
            std::cout << "Stopping thread pool..." << std::endl;
            for (auto &t: workers) {
                if (t.joinable()) t.join();
            }
        }

        Queue tasks;

    private:
        std::vector<std::thread> workers;
        std::atomic<bool> stop_flag;
    };
}
