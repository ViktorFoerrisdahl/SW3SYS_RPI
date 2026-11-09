#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>

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

    void publish(std::unique_ptr<Message> msg)
    {
        // TODO:
        // Avoid races when iterating through subscribers (lock)
        // Find subscribers for topic
        //  - copy to local vector
        // Notify all subscribers (Must this be inside or outside lock?)
        subscribers_mtx.lock();
        std::cout << "[Broker] Notifying for topic: " << msg->topic << std::endl;
        std::vector<std::function<void(const Message &)>> sub_temp;
        for (auto top : subscribers)
        {
            if (top.first == msg->topic)
            {
                sub_temp = subscribers.at(msg->topic);
            }
        }
        for (auto sub : sub_temp)
        {
            msg->data();
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

public:
    explicit Consumer(std::string n) : name(std::move(n)) {}

    void subscribeTopic(const std::string &topic)
    {
        Broker::getInstance().subscribe(topic, [this](const Message &msg)
                                        { this->onMessage(msg); });
    }

    void onMessage(const Message &msg) const
    {
        std::cout << "[Consumer " << name << "] received message on topic '"
                  << msg.topic << "': " << msg.data << "\n";
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

    std::thread t1([&]()
                   {
        for (int i = 0; i < 3; ++i) {
            
            p1.publishTopic("temperature", "Temperature is: " + std::to_string(rand()%40));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } });

    std::thread t2([&]()
                   {
        for (int i = 0; i < 3; ++i) {
            p2.publishTopic("humidity", "Humidity is: " + std::to_string(rand()%100));
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        } });

    t1.join();
    t2.join();
    return 0;
}
