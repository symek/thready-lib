//
// Created by symek on 4/8/25.
//
#include <vector>
#include <atomic>
#include <optional>

namespace thready {

    template<typename T>
    class RingBufferQueue {
    public:
        explicit RingBufferQueue(size_t capacity)
                : buffer(capacity), head(0), tail(0), cap(capacity) {}

        RingBufferQueue(RingBufferQueue &&other) noexcept
                : buffer(std::move(other.buffer)),
                  head(other.head.load()),
                  tail(other.tail),
                  cap(other.cap) {}

        RingBufferQueue(const RingBufferQueue &) = delete;

        RingBufferQueue &operator=(const RingBufferQueue &) = delete;


        bool push(const T &item) {
            size_t current_tail = tail;
            size_t next_tail = increment(current_tail);

            if (next_tail == head.load(std::memory_order_acquire)) {
                return false; // buffer full
            }

            buffer[current_tail] = item;
            tail = next_tail;
            return true;
        }

        bool pop(T &item) {
            size_t current_head = head.load(std::memory_order_relaxed);
            if (current_head == tail) {
                return false; // empty
            }

            item = buffer[current_head];
            head.store(increment(current_head), std::memory_order_release);
            return true;
        }

        bool empty() const {
            return head.load(std::memory_order_acquire) == tail;
        }

    private:
        size_t increment(size_t index) const {
            return (index + 1) % cap;
        }

        std::vector<T> buffer;
        std::atomic<size_t> head; // only consumers update
        size_t tail;              // only producer updates
        const size_t cap;
    };
}
