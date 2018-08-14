

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
/* Read the contents of FILENAME into a newly allocated buffer. The
size of the buffer is stored in *LENGTH. Returns the buffer, which
the caller must free. If FILENAME doesn’t correspond to a regular
file, returns NULL. */
char* read_file (const char* filename, size_t* length)
{
	int fd;
	struct stat file_info;
	char* buffer;
	/* Open the file. */
	fd = open (filename, O_RDONLY);
	/* Get information about the file. */
	fstat (fd, &file_info);
	*length = file_info.st_size;
	/* Make sure the file is an ordinary file.*/
	if (!S_ISREG (file_info.st_mode)) {
	/* It’s not, so give up. */
	close (fd);
	return NULL;
	}
	/* Allocate a buffer large enough to hold the file’s contents.*/
        if(*length<1)
	   return NULL;
	buffer = (char*) malloc (*length);
	/* Read the file into the buffer. */
	read (fd, buffer, *length);
	
	
	/* Finish up. */
	close (fd);
        
	return buffer;
}
int main(int argc, char *argv[])
{

  if(argc<2)
  {
	printf("Using STAT the program read the contents of FILENAME into a newly allocated buffer\n");
	printf("The path of the file to be read is required\n");
    	return 1;
   }
   char * filename = argv[0];
   size_t length;
   char * buffer = read_file(filename, &length);
   
   if (buffer != NULL)
     {
	int index =0;
        for(;index<length;++index)
        {
          printf("%c",(char)*(buffer+index));
          if((index % 24)==0)
	     printf("\n");
	}
	free(buffer);
     }  
   
   printf("\n%lu read into the buffer \n",length);
   return 0;
}
