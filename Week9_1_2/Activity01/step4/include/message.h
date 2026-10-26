#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <stdexcept>
#include <variant>
#include <queue>
#include <mutex>
#include <condition_variable>

struct doubleData
{
    double value;
};

struct intData
{
   int value;
};

using MessageData = std::variant<intData, doubleData>;

struct Message
{
    MessageData data; // data object
};

template <typename T>
class MessageQueue
{
public:
    MessageQueue(unsigned long maxSize) : maxSize_(maxSize) {}

    void push(T msg)
    {
        std::unique_lock<std::mutex> lock(lock_);
        ready.wait(lock, [this] {return msgQueue.size() <= maxSize_; });
        msgQueue.push_back(msg);
        lock.unlock();
        ready.notify_one();
    }

    void pop(T &msg)
    {
        std::unique_lock<std::mutex> lock(lock_);
        ready.wait(lock, [this] {return !msgQueue.empty(); });
        msg = msgQueue.front();
        msgQueue.pop_front();
        lock.unlock();
        ready.notify_one();
    }

private:
    unsigned long maxSize_;
    std::deque<T> msgQueue;
    std::condition_variable ready;
    std::mutex lock_;
};

#endif