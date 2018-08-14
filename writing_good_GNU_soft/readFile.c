
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main (int argc, char* argv[])
{

        if(argc < 2)
	{
	  printf ("This prgoram reads and print the content of hexadecimal file \n");
	  printf ("Program needs the path of the file to be read \n");
          return 1;
	}
	unsigned char buffer[16];
	size_t offset = 0;
	size_t bytes_read;
	int i;
	/* Open the file for reading. */
	int fd = open (argv[1], O_RDONLY);
	/* Read from the file, one chunk at a time. Continue until read
	"comes up short", that is, reads less than we asked for.
	This indicates that we’ve hit the end of the file. */
	do {
	/* Read the next line’s worth of bytes. */
	bytes_read = read (fd, buffer, sizeof (buffer));
	/* Print the offset in the file, followed by the bytes themselves.*/
	printf ("0x%06x : ", offset);
	for (i = 0; i < bytes_read; ++i)
	printf ("%02x ", buffer[i]);
	printf ("\n");
	/* Keep count of our position in the file. */
	offset += bytes_read;
	}
	while (bytes_read == sizeof (buffer));
	/* All done.*/
	close (fd);
	return 0;

}
