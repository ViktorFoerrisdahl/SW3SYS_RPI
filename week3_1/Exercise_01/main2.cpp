#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[]) 
{
    int fd = open("Ovelse1.txt", O_APPEND | O_RDWR);

    //Error handling
    if (fd == -1)
    {
        perror("Opening the file has failed :(");
        return -1;
    }

    char text[] = " Du er smuk i idag <3!";

    ssize_t BytesRead = write(fd, text, sizeof(text) -1);

    //Error handling
    if (BytesRead == -1)
    {
        perror("Writing to the file has failed :(");
        return -1;
    } 
    else if (BytesRead != sizeof(text))
    {
        perror("Ikke alt text blev writed");
        return -1;
    }

    //close file:
    close(fd);

    return 0;
}