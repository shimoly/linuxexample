#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
/* 
Returns the file descriptor for a newly created temporary file.The temporary file will be readable and writable by the effective
user ID of the current process but will not be readable or writable by anybody else.Returns -1 if the temporary file could not be created.
*/
int secure_temp_file()
{
    /* This file descriptor points to /dev/random and allows us to get a good source of random bits. */
    static int random_fd = -1;
    /* A random integer. */
    unsigned int random;
    /* A buffer, used to convert from a numeric to a string representation of random. This buffer has fixed size, meaning that we potentially have a buffer overrun bug if the integers on this machine have a *lot* of bits. */
    char filename[128];
    /* The file descriptor for the new temporary file. */
    int fd;
    /* Information about the newly created file. */
    struct stat stat_buf;
    /* If we haven’t already opened /dev/random, do so now. not threadsafe.) */
    if (random_fd == -1)
    {
        /* Open /dev/random. Note that we’re assuming that /dev/random really is a source of random bits, not a file full of zeros placed there by an attacker. */
        random_fd = open("/dev/random", O_RDONLY);
        /* If we couldn’t open /dev/random, give up. */
        if (random_fd == -1)
        {
            printf("Error opening random file\n");
            return -1;
        }
    }
    /* Read an integer from /dev/random. */
    if (read(random_fd, &random, sizeof(random)) != sizeof(random))
    {
        printf("Error reading random file\n");
        return -1;
    }
    /* Create a filename out of the random number. */
    sprintf(filename, "/tmp/%u", random);
    /* Try to open the file. */
    fd = open(filename,
              /* Use O_EXECL, even though it doesn’t work under NFS. */
              O_RDWR | O_CREAT | O_EXCL,
              /* Make sure nobody else can read or write the file. */
              S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Error opening %s\n", filename);
        return -1;
    }
    printf("%s has been created\n", filename);
    /* Call lstat on the file, to make sure that it is not a symbolic link. */
    if (lstat(filename, &stat_buf) == -1)
    {
        printf("Error getting the stat \n");
        return -1;
    }
    /* If the file is not a regular file, someone has tried to trick us. */
    if (!S_ISREG(stat_buf.st_mode))
    {
        printf("The file is not a regular file \n");
        return -1;
    }
    /* If we don’t own the file, someone else might remove it, read it,or change it while we’re looking at it. */
    if (stat_buf.st_uid != geteuid() || stat_buf.st_gid != getegid())
    {
        printf("we don’t own the file, someone else might remove it, read it,or change it while we’re looking at it\n");
        return -1;
    }
    /* If there are any more permission bits set on the file, something’s fishy. */
    long mode = (long)(stat_buf.st_mode & ~(S_IRUSR | S_IWUSR));
    printf("%s a mode  %o %o\n", filename, stat_buf.st_mode, mode);
    if ((stat_buf.st_mode & ~(S_IRUSR | S_IWUSR)) != 0)
    {
        printf("there are any more permission bits set on the file, something’s fishy\n");
        return -1;
    }

    printf("%s has been created securly \n", filename);

    return fd;
}
int main(int argc, char const *argv[])
{
    secure_temp_file();
    {
        printf("\n");
        return 0;
    }
}
