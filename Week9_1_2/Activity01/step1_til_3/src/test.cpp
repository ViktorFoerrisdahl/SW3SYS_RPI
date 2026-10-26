#include <iostream>
#include <thread>
#include <variant>

#include "message.h"

int main()
{

    std::cout << "SYS week9 \n";

    //opretter queue med size:
    MessageQueue<Message> q1 (10);

    //opretter vores data, som skal sendes:
    PearData p1;
    p1.name = "pear 1";
    p1.weight = 239;

    //opretter vores message
    Message m1;
    m1.data = p1;
    //

    //opretter modtager
    std::thread modtagerThread([&] 
    {
        while (1)
        {
            Message tempMsg;
            q1.pop(tempMsg); 
            MessageData tempData = tempMsg.data;

            if (std::holds_alternative<PearData>(tempData))
            {
                PearData pearTemp = std::get<PearData>(tempData);
                std::cout << "pear name: " << pearTemp.name << ", pear weigt: " << pearTemp.weight << "\n";
            }

            if (std::holds_alternative<AppleData>(tempData))
            {
                AppleData appleTemp = std::get<AppleData>(tempData);
                std::cout << "apple name: " << appleTemp.name << ", apple weigt: " << appleTemp.weight << "\n";
            }
        }
    });

    //opretter sender
    std::thread senderThread([&] {q1.push(m1); });


    senderThread.join();
    modtagerThread.join();

    return 0;
}