#include <iostream>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[])
{
    int x = 10;

    for (int i = 0; i < 2; i++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            return -1;
        }
        else if (pid == 0)
        { // Child process
            x += 5;
            cout << i << ": In child process, x = " << x << "\n";
        }
        else
        { // Parent process
            x -= 3;
            cout << i << ": In parent process, x = " << x << "\n";
        }
    }
    return 0;
}