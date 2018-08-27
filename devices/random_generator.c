
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Return a random integer between MIN and MAX, inclusive.randomness from /dev/random. */

int generate_random_number(int min, int max)
{
    /* Store a file descriptor opened to /dev/random in a static variable. That way, we don’t need to open the file every time this function is called. */
    static int dev_random_fd = -1;
    char* next_random_byte;
    int bytes_to_read;
    unsigned random_value;
    /* Make sure MAX is greater than MIN.*/
    assert (max > min);
    /* If this is the first time this function is called, open a file descriptor to /dev/random. */
    if (dev_random_fd == -1) 
    {
     dev_random_fd = open ("/dev/random", O_RDONLY);
     assert (dev_random_fd != -1);
    }
    /* Read enough random bytes to fill an integer variable. */
    next_random_byte = (char*) &random_value;
    bytes_to_read = sizeof (random_value);
    /* Loop until we’ve read enough bytes. Because /dev/random is filled from user-generated actions, the read may block and may only return a single random byte at a time. */
    do 
    {
        int bytes_read;
        bytes_read = read (dev_random_fd, next_random_byte, bytes_to_read);
        bytes_to_read -= bytes_read;
        next_random_byte += bytes_read;
    }while (bytes_to_read > 0);
    /* Compute a random number in the correct range.*/
    return min + (random_value % (max - min + 1)); 
}
struct option {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
};
int get_opt_arg(char ch)
{
    int value = -9;
    value = atoi(optarg);
    //printf("%c optarg is %s\n", ch, optarg);
    return value;
}
int
main(int argc, char *argv[])
{
    int min = 10, max = 0, num = 256;
    int opt;

    
   
    while ((opt = getopt(argc, argv, "m:x:n:")) != -1) {
        switch (opt) {
        case 'm':            
            min = get_opt_arg(opt);
            break;
        case 'x':
             max = get_opt_arg(opt);
           
            break;
        case 'n':
            num = get_opt_arg(opt);
            
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-n number] [-m min] [-x max]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

   printf("optind=%d\n", optind);
/*
   if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }*/
   printf("Generating radom for : %d Min : %d Max = %d\n", num, min, max);

   int counter = 0;
   int random_number;
   while (counter < num)
   {
        random_number = generate_random_number(min, max);        
        printf("%d th random number is %d\n",counter, random_number);
        counter++;
   }

   exit(EXIT_SUCCESS);
}