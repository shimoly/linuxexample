
#include <pthread.h>
#include <stdio.h>

int main()
{
	

	//Different mutex related API
	//
	pthread_mutexattr_t attr;
	
	pthread_mutex_t mutex;
	pthread_mutexattr_init (&attr);

	/* np shows it is not portable */
	pthread_mutexattr_setkind_np (&attr, PTHREAD_MUTEX_ERRORCHECK_NP);'

	pthread_mutex_init (&mutex, &attr);
	pthread_mutexattr_destroy (&attr);


	//pthread_mutex_trylock works as pthread_mutex_lock if the mutex is unlocked
	//if the the mutex is locked, it will not block, It will return immediately with the error code EBUSY
	pthread_mutex_trylock(&mutex)

}


