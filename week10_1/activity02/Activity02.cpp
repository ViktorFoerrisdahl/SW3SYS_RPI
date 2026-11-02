#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>

std::mutex std_lock;

template <typename T>
class BlockingQueue
{
public:
    BlockingQueue()
    {
        maxSize_ = 3;
    }

    void push(T msg)
    {
        std::unique_lock<std::mutex> lock(lock_);
        ready.wait(lock, [this]
                   { return msgQueue.size() != maxSize_; });
        msgQueue.push_back(msg);
        lock.unlock();
        ready.notify_one();
    }

    void pop(T &msg)
    {
        std::unique_lock<std::mutex> lock(lock_);
        ready.wait(lock, [this]
                   { return !msgQueue.empty(); });
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

// ---------------------------
// Message
// ---------------------------
struct Message
{
    std::string topic;
    std::string data;
};

// ---------------------------
// Broker (Singleton)
// ---------------------------
class Broker
{
private:
    std::map<std::string, std::vector<std::function<void(const Message &)>>> subscribers;
    std::mutex subscribers_mtx; // protects subscribers
    Broker() = default;

public:
    Broker(const Broker &) = delete; // Delete copy constructors
    Broker &operator=(const Broker &) = delete;

    static Broker &getInstance()
    {
        static Broker instance;
        return instance;
    }

    void subscribe(const std::string &topic, std::function<void(const Message &)> handler)
    {
        // TODO:
        // Avoid races when writing to subscribers (lock)
        // add handler to subscribers
        subscribers_mtx.lock();
        if (subscribers.contains(topic))
        {
            subscribers.at(topic).push_back(handler);
        }
        else
        {
            subscribers.try_emplace(topic);
            subscribers.at(topic).push_back(handler);
        }
        subscribers_mtx.unlock();
    }

    void publish(const std::string &topic, const Message &message)
    {
        // TODO:
        // Avoid races when iterating through subscribers (lock)
        // Find subscribers for topic
        //  - copy to local vector
        // Notify all subscribers (Must this be inside or outside lock?)
        subscribers_mtx.lock();
        {
            std_lock.lock();
            std::cout << "[Broker] Notifying for topic: " << topic << std::endl;
            std_lock.unlock();
        }
        std::vector<std::function<void(const Message &)>> sub_temp;
        for (auto top : subscribers)
        {
            if (top.first == topic)
            {
                sub_temp = subscribers.at(topic);
            }
        }
        for (auto sub : sub_temp)
        {
            sub(message);
        }
        subscribers_mtx.unlock();
    }
};

// ---------------------------
// Consumer
// ---------------------------
class Consumer
{
    std::string name;
    BlockingQueue<Message> queue;
    bool running = true;
    std::thread worker; // To host thread inside instance of Consumer

    void workerThread()
    {
        while (running)
        {
            // TODO: pop and dispatch messages
            Message temp_msg;
            queue.pop(temp_msg);
            dispatchMessage(temp_msg);
        }
    }

public:
    explicit Consumer(std::string n) : name(std::move(n))
    {
        worker = std::thread([this]
                             { workerThread(); });
    }
    ~Consumer()
    {
        worker.join();
    }

    void dispatchMessage(const Message &msg)
    {
        std_lock.lock();
        // TODO: Just print the message and topic
        std::cout << "[Consumer " << name << "] received message on topic '"
                  << msg.topic << "': " << msg.data << "\n";
        if (msg.topic == "shutdown_now")
        {
            std::cout << name << "'s WorkerThread is now shutting down! \n";
            running = false;
        }
        std_lock.unlock();
    }

    void subscribeTopic(const std::string &topic)
    {
        Broker::getInstance().subscribe(topic, [this](const Message &msg)
                                        { this->onMessage(msg); });
    }

    void onMessage(const Message &msg)
    {
        queue.push(msg);
    }
};

// ---------------------------
// Producer
// ---------------------------
class Producer
{
public:
    void publishTopic(const std::string &topic, const std::string &data) const
    {
        Message msg{topic, data};
        Broker::getInstance().publish(topic, msg);
    }
};

// ---------------------------
// Example Usage
// ---------------------------
int main()
{
    Producer p1, p2;
    Consumer c1("Heater"), c2("Fan"), c3("Window");

    c1.subscribeTopic("temperature");
    c2.subscribeTopic("temperature");
    c2.subscribeTopic("humidity");
    c3.subscribeTopic("humidity");

    c1.subscribeTopic("shutdown_now");
    c2.subscribeTopic("shutdown_now");
    c3.subscribeTopic("shutdown_now");

    std::thread t1([&]()
                   {
        for (int i = 0; i < 3; ++i) {
            
            p1.publishTopic("temperature", "Temperature is: " + std::to_string(rand()%40));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } });

    std::thread t2([&]()
                   {
        for (int i = 0; i < 3; ++i) 
        {
            p2.publishTopic("humidity", "Humidity is: " + std::to_string(rand()%100));
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        } });

    std::thread t3([&]()
                   {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            p2.publishTopic("shutdown_now", "shutdown_now"); });

    t1.join();
    t2.join();
    t3.join();
    return 0;
}
