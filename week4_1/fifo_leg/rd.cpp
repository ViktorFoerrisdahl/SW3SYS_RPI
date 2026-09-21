#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <unistd.h>

int main()
{
    std::thread::id id = std::this_thread::get_id(); // Get unique thread ID

    char buffer[20];
    buffer[0] = 'a';

    /* [Do forever] */
    while (1)
    {

        /*  [Open file 'testfifo' using std::ifstream] */
        std::ifstream file;
        file.open("/home/au786842/sw3sys/SW3SYS_RPI/week4_1/fifo_leg/testfifo");

        if (!file.is_open())
        {
            std::cerr << "Kunne ikke åbne filen!" << std::endl;
        }

        /*  [Read line from file with std::getline] */
        file.getline(buffer, 20);

        /*  [Print 'id' and line using std::cout] */
        std::cout << "id: " << id << " line: " << buffer;
        // for (auto i = 0; i < 20; i++)
        // {
        //     std::cout << buffer[i];
        // }
        std::cout << std::endl;
        sleep(1);
    }

    return 0;
}