#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define FILE_LENGTH 0x100
int main(int argc, char *const argv[])
{
    int fd;
    void *file_memory;
    int integer = 109;

    if (argc < 2)
    {
        printf("Read a Random Number to a Memory-Mapped File \n");
        printf("The path of the file is required\n");
        return 1;
    }

    /* Open the file. */
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    /* Create the memory mapping. */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Read the integer, print it out, and double it. */

    int counter = 0;
    while (counter++ < 60)
    {
        sscanf(file_memory, "%d", &integer);
        printf("value: %d\t%p\n", integer, file_memory);
        sprintf((char *)file_memory, "%d\n", 2 * integer);

        sleep(1);
        /* Release the memory (unnecessary because the program exits).*/
    }
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
