#include <iostream>
#include <fstream>
#include <string>
#include <thread>

int main()
{
    std::thread::id id = std::this_thread::get_id(); // Get unique thread ID

    char buffer[20];

    /* [Do forever] */
    while (1)
    {
        /*  [Open file 'testfifo' using std::ifstream] */
        std::ifstream file;
        file.open("week4_1/fifo_leg/testfifo");

        /*  [Read line from file with std::getline] */
        file.getline(buffer, 20);

        /*  [Print 'id' and line using std::cout] */
        std::cout << "id: " << id << " line: ";
        for (auto i = 0; i < 20; i++)
        {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

        //close file again
        file.close();
    }

    return 0;
}