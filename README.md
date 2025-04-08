## 🧵 Thread Pool Variants

This library provides multiple thread pool implementations optimized for different use cases. All share a common interface via CRTP (`ThreadPoolBase`), and can be instantiated with different queue types depending on performance and behavior needs.

| ThreadPool Type       | Suitable Queues                       | Wait Strategy                      | Notes                                           |
|-----------------------|----------------------------------------|------------------------------------|------------------------------------------------|
| `ThreadPoolBlocking`  | `MutexQueue`, `std::queue`            | `std::condition_variable`          | Classic design, low CPU usage under idle load  |
| `ThreadPoolSpinning`  | `RingBufferQueue`, `LockFreeQueue`    | Spin-wait using `std::this_thread::yield()` | Lock-free and fast, but may burn CPU           |
| `ThreadPoolHybrid`    | `RingBufferQueue`                     | Blocking with `condition_variable` + atomics | Best of both worlds — fast and energy-efficient |

### 🔧 Choose the right pool

- **Use `ThreadPoolBlocking`** for general workloads where task rate is low or bursty.
- **Use `ThreadPoolSpinning`** when maximum throughput is critical and you're OK with busy-waiting.
- **Use `ThreadPoolHybrid`** if you're using a bounded queue (like `RingBufferQueue`) and want high performance **without** CPU burn.

All pools support the same interface:

```cpp
pool.enqueue([] { /* your task */ });
