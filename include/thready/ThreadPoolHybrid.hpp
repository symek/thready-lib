//
// Created by symek on 4/8/25.
//
#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <type_traits> // for std::is_constructible_v

namespace thready {


    template<typename Queue, typename TaskType = std::function<void()>>
    class ThreadPoolHybrid {
    public:
        explicit ThreadPoolHybrid(size_t thread_count, size_t queue_capacity = 0)
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
                    while (true) {
                        TaskType task;
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this]() {
                                return stop_flag.load() || !tasks.empty();
                            });

                            if (stop_flag.load() && tasks.empty()) {
                                return;
                            }

                            if (!tasks.pop(task)) {
                                continue; // spurious wakeup or race
                            }
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPoolHybrid() {
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                stop_flag = true;
            }
            condition.notify_all();
            for (auto &t: workers) {
                if (t.joinable()) t.join();
            }
        }

        template<typename Func>
        bool enqueue(Func &&f) {
            bool success = tasks.push(TaskType(std::forward<Func>(f)));
            if (success) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                condition.notify_one();
            }
            return success;
        }

    private:
        std::vector<std::thread> workers;
        Queue tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::atomic<bool> stop_flag;
    };

}

