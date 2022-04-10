#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace ecapplogcpp
{

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue(void)
        : q()
        , m()
        , c()
    {}

    ~SafeQueue(void)
    {}

    // Clear
    void clear()
    {
        std::lock_guard<std::mutex> lock(m);
        while (!q.empty()) q.pop_front();
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push_back(t);
        c.notify_one();
    }

    // Add an element to the queue at the top.
    void reenqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push_front(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
        }
        T val = q.front();
        q.pop_front();
        return val;
    }

private:
    std::deque<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

}
