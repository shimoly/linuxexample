#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
    int min = -1, max = -1, num = -1;
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
   printf("Number : %d Min : %d Max = %d\n", num, min, max);

   /* Other code omitted */

   exit(EXIT_SUCCESS);
}