#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* A handle for temporary file created with the write_temp _file. In this implementation, it is just a descriptor */

typedef int temp_file_handle;

/* Write LENGTH byte from BUFFER into a temporary file. The temporary file is immediately unlinked. Returns a handle to the temporary file*/


temp_file_handle write_file(char * buffer, size_t length)
{
	
	/* Create a filename and file. The XXXXXX will be replaced with charactes that makes the filename unique*/
	
	char temp_filename[] = "/tmp/temp_file.XXXXXX";
	int fd = mkstemp(temp_filename);
	
	/* Unlink the file immediately, so that it will be removed when the file descritor is closed*/
	unlink(temp_filename);
	/* Write the number of byte to the file first*/
	
	write(fd, &length, sizeof(length));
	/* Now write the data itself*/
	write(fd, buffer, length);
	
	/* Use the file descriptor as the handle for the temporary file. */

	return fd;
}


/* Read the content of temporary file TEMP_FILE Created with the write_temp_file. The return value is a newly allocated buffer of those contents, which the caller must deallocate with free. *LENGTH is set to the size of the contents, in bytes. The temporary file is removed*/

char *read_temp_file(temp_file_handle temp_file, size_t * length)

{
	char *buffer;
	/*The TEMP_FILE handle is the a file descriptor to the temporary file. */
	int fd = temp_file;
	/* Rewind to the begining of the file*/
	lseek(fd, 0, SEEK_SET);
	/* Read the size of the data in the temprary file*/
	read(fd, length, sizeof(length));
	/*Allocate buffer and read the data*/
	buffer = (char *)malloc(*length);
	read(fd, buffer, *length);
	
	/*Close the descriptor which makes the temorary file got away */
	close(fd);
	return buffer;
	

}

int main()
{
   char buffer [] = "Testing writing to file and reading it back, ";
   printf("WRITE: (length = %lu)\t%s\n",sizeof(buffer), (char *)&buffer);
   temp_file_handle fd= write_file((char *)&buffer[0], sizeof(buffer));
   size_t read_length;
   char * read_buffer = read_temp_file(fd, &read_length);
   printf ("READ: (length = %lu)\t%s\n",read_length, read_buffer);
  
	
}
