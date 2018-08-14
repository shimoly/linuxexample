#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>
/* Allocate a temporary buffer.*/
void* allocate_buffer (size_t size)
{
  return malloc (size);
}

/* Deallocate a temporary buffer.*/
void deallocate_buffer (void* buffer)
{

      printf("freeing \n");
      free (buffer);
}

void do_some_work ()
{
   /* Allocate a temporary buffer.*/
   void* temp_buffer = allocate_buffer (1024);
   /* Register a cleanup handler for this buffer, to deallocate it in case the thread exits or is cancelled. */
   pthread_cleanup_push (deallocate_buffer, temp_buffer);
   /* Do some work here that might call pthread_exit or might be cancelled... */
  /* Unregister the cleanup handler. Because we pass a nonzero vlue,this actually performs the cleanup by calling
 deallocate_buffer. */
  pthread_cleanup_pop (1);
}

void * thread_function(void *args)
{
	//do_some_work();
	printf("working \n");
	sleep(5);
	do_some_work();
}
int main()
{
	pthread_t pthread_id;
	pthread_create(&pthread_id, NULL, thread_function, NULL);
	pthread_join(pthread_id, NULL);
	return 0;
}
