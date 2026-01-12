#pragma once
// Include thread support
#include <thread>
// Include functional for std::function
#include <functional>
// Include atomic for atomic flags
#include <atomic>
// Include thread-safe queue header
#include "ThreadSafeQueue.h"

template<typename Msg>
class ActiveObject {
public:
    // Type alias for processor function
    using Processor = std::function<void(Msg&&, ThreadSafeQueue<Msg>* out)>;

private:
    // Pointer to input queue
    ThreadSafeQueue<Msg>* in;
    // Pointer to output queue
    ThreadSafeQueue<Msg>* out;
    // Processor function
    Processor proc;
    // Thread for running the active object
    std::thread th;
    // Atomic flag for running state
    std::atomic<bool> running{false};

public:
    // Constructor initializes queues and processor
    ActiveObject(ThreadSafeQueue<Msg>* inQ,
                 ThreadSafeQueue<Msg>* outQ,
                 Processor p)
        : in(inQ), out(outQ), proc(std::move(p)) {}

    // Start the active object's thread
    void start() {
        running = true;
        th = std::thread([this]{
            // Main loop: process messages while running
            while (running) {
                auto item = in->pop();
                // If queue is closed, exit loop
                if (!item.has_value()) break;
                // Process the message and send to output queue
                proc(std::move(*item), out);
            }
            // Close output queue when done
            if (out) out->close();
        });
    }

    // Stop the active object and close input queue
    void stop() {
        running = false;
        if (in) in->close();
    }

    // Join the thread if joinable
    void join() { if (th.joinable()) th.join(); }
};
