#include <iostream>
#include <thread>
#include <variant>

#include "message.h"

static void handle(const intData& data)
{
    std::cout << "Int value: " << data.value << "\n";
}

static void handle(const doubleData& data)
{
    std::cout << "double value: " << data.value << "\n";
}

int main()
{

    std::cout << "SYS week9 \n";

    //opretter queue med size:
    MessageQueue<Message> q1 (10);

    //opretter vores data, som skal sendes:
    intData i1;
    i1.value = 23;

    intData i2;
    i2.value = 233;

    intData i3;
    i3.value = 2333;

    intData i4;
    i4.value = 23333;

    intData i5;
    i5.value = 233333;

    intData i6;
    i6.value = 2333333;
    
    doubleData d1;
    d1.value = 23.23;


    //opretter vores message
    Message m1;
    m1.data = i1;

    Message m2;
    m2.data = i2;

    Message m3;
    m3.data = d1;

    Message m4;
    m4.data = i3;

    Message m5;
    m5.data = i4;

    Message m6;
    m6.data = i5;

    Message m7;
    m7.data = i6;
    

    //opretter modtager
    std::thread modtagerThread([&] 
    {
        while (1)
        {
            Message tempMsg;
            q1.pop(tempMsg); 
            std::visit([](auto&& data) {handle(data); }, tempMsg.data);
        }
    });

    //opretter sender
    std::thread senderThreadInt([&] {
        q1.push(m1);
        q1.push(m2);
        q1.push(m4);
        q1.push(m5);
        q1.push(m6);
        q1.push(m7);
    });

      std::thread senderThreadDuuble([&] {
        q1.push(m3);
    });

    senderThreadDuuble.join();
    senderThreadInt.join();
    modtagerThread.join();

    return 0;
}