#include <stdio.h>
#include <assert.h>

static int print_args(int index, char* name, char*value);

/*
 * USE -DNDEBUG makes the assert macro will be processed away
 * important for performance critical source files
 *
 * */
int main(int argc, char*argv[])
{
	assert(argc>=2);
        print_args(1,"Program Name", argv[0]);
	print_args(2,"First arg", argv[1]);
        print_args(3, "Second arg", argv[2]);	
	return 0;
}

static int print_args(int index,char * name, char *value)
{
   printf("%s %d:\t%s\n",name, index, value);
   return 0;
}
