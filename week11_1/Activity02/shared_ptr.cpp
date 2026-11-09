#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>

// ---------------------------
// Message
// ---------------------------
struct Message
{
    std::string topic;
    std::string data;
};

// ---------------------------
// Consumer
// ---------------------------
class Consumer
{
    std::string name;

public:
    explicit Consumer(std::string n) : name(std::move(n)) {}

    void onMessage(const Message &msg)
    {
        std::cout << "[Consumer " << name << "] received '" << msg.data
                  << "' on topic '" << msg.topic << "'\n";
    }
};

// ---------------------------
// Broker
// ---------------------------
class Broker
{
private:
    std::map<std::string, std::vector<std::weak_ptr<Consumer>>> subscribers;
    std::mutex mtx;
    Broker() = default;

public:
    Broker(const Broker &) = delete;
    Broker &operator=(const Broker &) = delete;

    static Broker &getInstance()
    {
        static Broker instance;
        return instance;
    }

    void subscribe(const std::string &topic, std::weak_ptr<Consumer> consumer)
    {
        std::lock_guard<std::mutex> lock(mtx);
        subscribers[topic].push_back(consumer);
    }

    void publish(const std::string &topic, const Message &msg)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);

            // Look up Topic in subscribers
            auto subscribers_it = subscribers.find(topic);
            if (subscribers_it == subscribers.end())
            {
                std::cout << "[Broker] No subscribers for topic: " << topic << "\n";
                return;
            }

            // Publish to all Consumers of topic
            for (std::weak_ptr<Consumer> c : subscribers_it->second)
            {
                if (auto d = c.lock())
                {
                    d->onMessage(msg);
                }
            }
        }   
    }

    void unsubscribe(const std::string &topic, std::shared_ptr<Consumer> consumer) {}
};

// ---------------------------
// Example Usage
// ---------------------------
int main()
{

    // TODO: Define consumers "Heater", "Fan" and "Window" as shared pointers

    std::shared_ptr<Consumer> c1 = std::make_shared<Consumer>("Heater");

    std::shared_ptr<Consumer> c2 = std::make_shared<Consumer>("Fan");

    std::shared_ptr<Consumer> c3 = std::make_shared<Consumer>("Window");

    // TODO: Subscribe to topics "temperature" and "humidity"

    Broker::getInstance().subscribe("temperature", c1);

    Broker::getInstance().subscribe("humidity", c1);

    Broker::getInstance().subscribe("humidity", c2);

    Broker::getInstance().subscribe("temperature", c3);

    // TODO: Use shared_ptr::reset() to replace the Consumer with NULL and effectively deleting the consumer (but not the shared_ptr)

    c1.reset();
    // Reading the textual output, is the deleted Consumer no longer recieving messages?
    // vi har to ting, som peger på vores consumer, og vi sletter kun 1 ved reset. derfor lever vores shared pointer stadig


    //Reading the textual output, is the deleted Consumer no longer recieving messages?
    // den korrekte consumers pointers bliver slettet og modtager ikke længere beskeder

    //Why does changing the shared_ptr to weak_ptr change this behaviour?
    // Da weakptr kun skal slettes en gang for at blive slettet er det nok at vi sletter den i main
    
    // Why do we use shared_ptr in main and parameters, but weak_ptr in Broker?
    // Broker needs to be able to 

    // TODO: unsubscribe to topic (when unsubscribe is implemented)

    // Publish messages to Broker
    std::thread t1([&]()
                   {
        for (int i = 0; i < 3; ++i) {
            std::string str{"Temperature is: " + std::to_string(rand() % 40)};
            Message msg{"temperature", str};
            Broker::getInstance().publish(msg.topic, msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        } });

    std::thread t2([&]()
                   {
        for (int i = 0; i < 3; ++i) {
            std::string str{"Humidity is: " + std::to_string(rand() % 100)};
            Message msg{"humidity", str};
            Broker::getInstance().publish(msg.topic, msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        } });

    t1.join();
    t2.join();
    return 0;
}