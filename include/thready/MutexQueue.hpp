//
// Created by symek on 4/8/25.
//
#pragma once
#include <queue>
#include <mutex>

template<typename T>
class MutexQueue {
public:
    bool push(const T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(item);
        return true;
    }

//    bool push(T&& item) {
//        std::lock_guard<std::mutex> lock(mtx);
//        q.push(std::move(item));
//        return true;
//    }
//
//    bool emplace(T&& item) {
//        std::lock_guard<std::mutex> lock(mtx);
//        q.push(std::move(item));
//        return true;
//    }



    bool pop(T& item) {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return false;
        item = std::move(q.front());
        q.pop();
        return true;
    }

    // Add front member function
    T &front() {
        std::lock_guard<std::mutex> lock(mtx);
        return q.front();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

private:
    std::queue<T> q;
    mutable std::mutex mtx;
};

