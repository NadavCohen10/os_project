// ThreadSafeQueue.h
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class ThreadSafeQueue {
    // Internal queue to store items
    std::queue<T> q;
    // Mutex for synchronizing access
    std::mutex m;
    // Condition variable for blocking/waking threads
    std::condition_variable cv;
    // Flag to indicate if queue is closed
    bool closed = false;

public:
    // Push an item into the queue
    void push(T v) {
        {
            std::lock_guard<std::mutex> lk(m);
            // If queue is closed, do not push
            if (closed) return;
            // Add item to queue
            q.push(std::move(v));
        }
        // Notify one waiting thread
        cv.notify_one();
    }

    // Pop an item from the queue, returns std::nullopt if closed and empty
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lk(m);
        // Wait until queue is not empty or closed
        cv.wait(lk, [&]{ return closed || !q.empty(); });
        // If queue is empty, return nullopt
        if (q.empty()) return std::nullopt;
        // Get item from front of queue
        T v = std::move(q.front());
        // Remove item from queue
        q.pop();
        // Return the item
        return v;
    }

    // Close the queue and notify all waiting threads
    void close() {
        {
            std::lock_guard<std::mutex> lk(m);
            closed = true;
        }
        cv.notify_all();
    }
};
