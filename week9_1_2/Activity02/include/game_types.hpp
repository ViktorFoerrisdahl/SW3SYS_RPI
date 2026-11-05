#pragma once

#include <vector>
#include <variant>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <mutex>
#include <condition_variable>

struct Vec2 
{
    int x, y;
};

struct BulletData {
    std::vector<Vec2> bullets;
};

struct EnemiesData {
    std::vector<Vec2> enemies;
};

using MessageData = std::variant<BulletData, EnemiesData>;

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
        ready.wait(lock, [this] {return msgQueue.size() != maxSize_; });
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
