#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * print_xs(void *unused)
{
	while(1)
	{
		fputc('x',stderr);
		sleep(1);
	}
	return NULL;
}
int main()

{
	pthread_t thread_id;
	//Create a new thread.
	//The new thread will run the print_xs function
	//
	
	pthread_create(&thread_id, NULL, &print_xs, NULL);
	while(1)
	{
		fputc('0', stderr);
		sleep(1);
	}
	return 0;
}
