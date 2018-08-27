#include <stdio.h>

int main (int argc, char* argv[])

{
	
    char buff[100];
    int x;
    if(argc!=2)	
		exit(1);	
    x = 1;
    snprintf(buff, sizeof(buff), argv[1]);
    buff[sizeof(buff)-1] = 0;
    printf("buffer (%d): %s\n",strlen(buff), buff);
    printf("x is %d/%x (@ %p)\n", x, x, &x);

    return 0;

}