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
#include <queue>


namespace thready {

    template<typename Queue, typename TaskType = std::function<void()>>
    class ThreadPoolBlocking {
    public:
        explicit ThreadPoolBlocking(size_t thread_count)
                : stop_flag(false) {
            for (size_t i = 0; i < thread_count; ++i) {
                workers.emplace_back([this]() {
                    while (true) {
                        TaskType task;
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this]() {
                                return stop_flag || !tasks.empty();
                            });

                            if (stop_flag && tasks.empty())
                                return;

                            task = std::move(tasks.front());
                            tasks.pop(task);
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPoolBlocking() {
            {
                std::scoped_lock lock(queue_mutex);
                stop_flag = true;
            }
            condition.notify_all();
            for (std::thread &t: workers) {
                if (t.joinable()) t.join();
            }
        }

        template<typename Func>
        void enqueue(Func &&f) {
            {
                std::scoped_lock lock(queue_mutex);
                tasks.push(std::forward<Func>(f));
            }
            condition.notify_one();
        }

//        bool has_work() {
//            std::scoped_lock lock(queue_mutex);
//            return !tasks.empty();
//        }

    private:
        std::vector<std::thread> workers;
        Queue tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::atomic<bool> stop_flag;
    };
}
