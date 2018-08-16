
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

/* Return a uniformly random number in the range [low, high] */

int random_range(unsigned const low, unsigned const high)
{
	unsigned const range = high -low +1;
	return low + (int)(((double)range)*rand()/(RAND_MAX+1.0));

}

int main(int argc, char * const argv[])
{
	int fd;
	void * file_memory;

	if(argc<2)
	{
         printf("Write a Random Number to a Memory-Mapped File \n");
         printf("The path of the file is required\n");
         return 1;
   	}

	/* Seed the random generator*/

	srand(time(NULL));

	/* Prepare a file large enoungh to hold unsigned integer */

	fd = open(argv[1], O_RDWR| O_CREAT, S_IRUSR| S_IWUSR);

	lseek(fd, FILE_LENGTH +1, SEEK_SET);

	write(fd, "", 1);

	lseek(fd, 0, SEEK_SET);

	/* Create the memory mapping  */

	file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0); 

	close(fd);

	/* Write a random integer to memory mapped area*/

	int random_number;
        
	int counter = 0;
	while(counter++ <60)
	{
		random_number = random_range(-100, 100);

	printf("Value to be written: %d \n", random_number);
		
	sprintf((char *)file_memory, "%d\n",random_number);

	sleep(1);
	}
	/*release the memory (unnecessary because the program exits)*/

	munmap(file_memory, FILE_LENGTH);

	return 0;
}  
