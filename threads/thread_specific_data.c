#include <malloc.h>
#include <stdio.h>
#include <pthread.h>


/* The key used to associate a log file pointer with each thread */

static pthread_key_t thread_log_key;

/*Write message to the log file for the current thread */

void write_to_thread_log(const char *message)
{
	FILE* thread_log = (FILE *)pthread_getspecific(thread_log_key);
	fprintf(thread_log, "%s\n", message);
}

/*Close the log file pointer THREAD_LOG */

void close_thread_log(void *thread_log)
{
	printf("Closing .... %d\n", (int)pthread_self());
	fclose((FILE *)thread_log);

}

void * thread_function(void *args)
{
	char thread_log_file_name[20];
	FILE * thread_log;
	/*Generate a file name for this thread's log file*/
	sprintf(thread_log_file_name, "thread%d.log", (int)pthread_self());
	//Open the file
	thread_log = fopen(thread_log_file_name, "w");

	pthread_setspecific (thread_log_key, thread_log);
	write_to_thread_log("Thread starting .....");


	return NULL;

}

int main()

{
	int i;
	pthread_t threads[5];

	/* Create a key to associate thread log file pointers in thread-specific data.
	 * use close_thread_log to clean up the file pointers */
	pthread_key_create(&thread_log_key, close_thread_log);

	//create threads to do the work
	for (i=0;i<5;++i)
		pthread_create(&(threads[i]),NULL,thread_function,NULL);
	for(i=0;i<5;++i)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}
