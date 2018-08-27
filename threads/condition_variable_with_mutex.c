#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

void initialize_flag()
{
	/* Initialize the mutex and condition variable.*/
	pthread_mutex_init(&thread_flag_mutex, NULL);
	pthread_cond_init(&thread_flag_cv, NULL);

	/* Initialize the flag value. */
	thread_flag = 0;
}

void do_work(int thread_id, int flag_value)
{
	/*int index = 0;
	for (; index < thread_id; ++index)
	{
		printf("\t");
	}*/
	printf("%d\t%d\n",thread_id, flag_value);
}
/* Calls do_work repeatedly while the thread flag is set; blocks if the flag is clear. */
void *thread_function(void *thread_arg)
{
	/* Loop infinitely. */

	int *thread_id = (int *)thread_arg;
	//printf("Starting thread %d\n", *thread_id);
	int flag_value = 0;
	while (1)
	{
		/* Lock the mutex before accessing the flag value. */
		pthread_mutex_lock(&thread_flag_mutex);
		while (!thread_flag)
			/* The flag is clear. Wait for a signal on the condition variable, indicating that the flag value has changed. * When the signal arrives and this thread unblocks, loop and check the flag again. */
			pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
		/* When we’ve gotten here, we know the flag must be set. Unlock the mutex. */
		flag_value = thread_flag;
		thread_flag = 0;
		pthread_mutex_unlock(&thread_flag_mutex);
		/* Do some work. */
		do_work(*thread_id, flag_value);
	}
	return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE.*/
void set_thread_flag(int flag_value)
{
	/* Lock the mutex before accessing the flag value. */
	pthread_mutex_lock(&thread_flag_mutex);
	/* Set the flag value, and then signal in case thread_function is blocked, waiting for the flag to become set.*  However, thread_function can’t actually check the flag until the mutex is unlocked. */

	thread_flag = flag_value;
	pthread_cond_signal(&thread_flag_cv);
	/* Unlock the mutex. */
	pthread_mutex_unlock(&thread_flag_mutex);
}
void *signal_source(void *args)
{
	int current_flag = 0;
	printf("setting flag %d\n", current_flag);
	while (1)
	{
		set_thread_flag(current_flag++);
		//printf("setting flag %d\n", current_flag);
		sleep(5);
	}
}
int main(int argc, char const *argv[])
{
	int i;
	pthread_t threads[5];
	int thread_id[5] = {1, 2, 3, 4, 5};
	pthread_t signal_creator;

	pthread_create(&signal_creator, NULL, signal_source, NULL);
	//create threads to do the work

	for (i = 0; i < 5; ++i)
	{
		pthread_create(&(threads[i]), NULL, thread_function, (void *)(&thread_id[i]));
	}
	for (i = 0; i < 5; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}
