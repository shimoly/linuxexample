#include <stdio.h>

int traverse_till_first_special_char(char *source)
{
	char * begin = source;
	char * end = begin;
        char next_char = *end;
	while( next_char != ';'  && next_char != '.' && next_char != ' '  && next_char != '|' && next_char)
	{
	
		
		end += 1;
		next_char = *end;
	}
        *end = '\0';
	printf("Segmented: %s\n",begin);

}	
int main (int argc, char* argv[])

{
	if(argc<2)
	{
		printf("This program copy all the chars until it found one of special chars \r\n Please pass String\n");
		return 0;
	}
	printf("Original %s\n", argv[1]);
	return traverse_till_first_special_char(argv[1]);
}

