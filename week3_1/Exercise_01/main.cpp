#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]) 
{
    int fd = open("Ovelse1.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);

    //Error handling
    if (fd == -1)
    {
        perror("Opening the file has failed :(");
        return -1;
    }

    char text[] = "Hello shawjty!";

    ssize_t BytesRead = write(fd, text, sizeof(text) -1);

    //Error handling
    if (BytesRead == -1)
    {
        perror("Writing to the file has failed :(");
        return EXIT_FAILURE;
    } 
    else if (BytesRead != sizeof(text))
    {
        perror("Ikke alt text blev writed");
        return EXIT_FAILURE;
    }

    //close file:
    close(fd);

    return EXIT_SUCCESS;
}