#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
char *get_timestamp()
{
	time_t now = time(NULL);
	return asctime(localtime(&now));
}
int main (int argc, char* argv[])
{

        if(argc < 2)
	{
	  printf ("This programe appends the current time of the machine to to passed file Name \n");
	  printf ("Program needs file name to append the time \n");
	  return 1;
	}
	/* The file to append the current timestampt */
	char* filename = argv[1];
	/* Get the current time stamp */
	char * timestamp = get_timestamp ();
	/*Open the file for writing. If it exits, append to it other wise crate new one*/
	
	int fd = open (filename, O_WRONLY | O_CREAT |O_APPEND, 0666);
	/*calc the length*/
	size_t length = strlen(timestamp);
	write(fd, timestamp, length);
        close(fd);
	
	return 0;
}
