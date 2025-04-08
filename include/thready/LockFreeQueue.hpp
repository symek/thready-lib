//
// Created by symek on 4/8/25.
//
#include <memory>
#include <atomic>
#include <functional>

namespace thready {

    template<typename T>
    class LockFreeQueue {
    public:
        LockFreeQueue() {
            auto dummy = std::make_unique<Node>();
            head.store(dummy.get());
            tail.store(dummy.get());
            holder = std::move(dummy); // own the dummy node
        }

        void push(T value) {
            auto new_node = std::make_unique<Node>(std::move(value));
            Node* new_raw = new_node.get();

            while (true) {
                Node* last = tail.load();
                Node* next = last->next.load();

                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, new_raw)) {
                        tail.compare_exchange_weak(last, new_raw);
                        new_node.release(); // transfer ownership
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(last, next);
                }
            }
        }

        bool pop(T& result) {
            while (true) {
                Node* first = head.load();
                Node* next = first->next.load();
                if (next == nullptr) {
                    return false;
                }

                if (head.compare_exchange_weak(first, next)) {
                    result = std::move(next->value);
                    std::unique_ptr<Node> old(first); // cleanup old dummy
                    return true;
                }
            }
        }

        bool empty() const {
            Node* first = head.load();
            return first->next.load() == nullptr;
        }

        ~LockFreeQueue() {
            Node* node = head.load();
            while (node != nullptr) {
                Node* next = node->next.load();
                delete node;
                node = next;
            }
        }

    private:
        struct Node {
            T value;
            std::atomic<Node*> next;
            Node() : next(nullptr) {}
            explicit Node(T val) : value(std::move(val)), next(nullptr) {}
        };

        std::atomic<Node*> head;
        std::atomic<Node*> tail;
        std::unique_ptr<Node> holder; // holds the dummy node
    };
}
