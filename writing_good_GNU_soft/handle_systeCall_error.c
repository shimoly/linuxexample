#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
//This is set by the chown module
extern int errno;

int chown_with_full_error_codes(char * path, uid_t user_id)
{
  int rval = chown (path, user_id, -1);
  if (rval != 0) 
     {
	/* Save errno because it’s clobbered by the next system call. */
	int error_code = errno;
	/* The operation didn’t succeed; chown should return -1 on error. */
	assert (rval == -1);
	/* Check the value of errno, and take appropriate action. */
	switch (error_code) 
	{
	 case EPERM:		/* Permission denied. */
	 case EROFS:		/* PATH is on a read-only file system. */
	 case ENAMETOOLONG: 	/* PATH is too long. */
	 case ENOENT:		/* PATH does not exit. */
	 case ENOTDIR:		/* A component of PATH is not a directory. */
	 case EACCES:		/* A component of PATH is not accessible. */
	 /* Something’s wrong with the file. Print an error message. */
	 fprintf (stderr, "error changing ownership of %s: %s\n",
	 path, strerror (error_code));
	 /* Don’t end the program; perhaps give the user a chance to
	 choose another file... */
	 break;
	 case EFAULT:		 /* PATH contains an invalid memory address.	 This is probably a bug.*/
	 abort();
	 case ENOMEM:
	 /* Ran out of kernel memory. */
	 fprintf (stderr, "%s\n", strerror (error_code));
	 exit (1);
	 default:
	 /* Some other, unexpected, error code. We’ve tried to handle all
	 possible error codes; if we’ve missed one, that’s a bug! */
	 abort ();
	};
     
     }
}
int chown_with_assert(char * path, uid_t user_id)
{
	int rval = chown (path, user_id, -1);
	assert (rval == 0);
}

int main(int argc, char * argv[])
{
	if(argc<3)
	{
		printf("The programme need a path to set change the owner\n");
		printf("USAGE: %s \"Path to file\" \" user id\"\n",argv[0]);
		return 1;
	}
	char *endptr;
	uid_t uid = strtol(argv[2], &endptr, 10);
	
	return chown_with_full_error_codes(argv[1], uid);
	

}
