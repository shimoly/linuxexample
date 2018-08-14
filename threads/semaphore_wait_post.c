

#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

struct job
{
	/* Link field for linked list*/
	struct job *next;
	char job_name[10];

};

/* A listing list of pending jobs */


struct job* job_queue;

void enqueue_job(char* job_name);
/* A mutex protecting job_queue */


pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;


/* A non-zero value would indicate a semaphore that can be shared across processes, which is not supported by the GNU/Linux for this type of semaphore
 *
 *
 * A semaphore counting the number of jobs in the queue */

	sem_t job_queue_count;



/* Perform one-time initlization of the job queue*/

void initialize_job_queue()
{
	/* The queue is initlilly empty */
	
	job_queue = NULL;
	/* Initlize the semaphore which counts jobs in the queue. Its initial value should be zero */
	sem_init(&job_queue_count, 0 , 0);


}
void process_job(struct job *current_job)
{
	printf("%s\t%lu\n",current_job->job_name, (unsigned long)pthread_self());
}
void * thread_function(void *args)
{
	while(1)
	{
		struct job * next_job;
		/* Wait on the job queue semaphore. If this is positive 
		 * indicating the queue is not empty, decrement the count by 1.
		 * If the queue is empty, block until a new job is enqueued*/

		sem_wait(&job_queue_count);

		/*Lock the mutex on the job queue*/

		pthread_mutex_lock(&job_queue_mutex);

		/* Because of the semaphore we know that the job queue is not empty.
		 * Get the next available job*/

		next_job = job_queue;

		/*Remove this job from the job list*/

		job_queue = job_queue->next;

		/* Unlock the mutex on the job queue because we're done with the queue for now*/

		pthread_mutex_unlock(&job_queue_mutex);

		/* carry out the job*/

		process_job(next_job);

		/* clean up */

		free(next_job);

	}
	return NULL;

	}


void * job_creator(void *args)
{
	int job_number = 1;
	char job_name[50];
	while(1)
	{
	  sprintf(job_name,"J-%d", job_number++);
	  enqueue_job(job_name);
	  sleep(1);	
	}
}
void enqueue_job(char* job_name)
{
	struct job* new_job;

	

	/*Allocate a new object*/

	new_job = (struct job*) malloc(sizeof(struct job));
        strncpy(new_job->job_name, job_name, sizeof(new_job->job_name));

	/* Lock the mutex on the job befor accessing it*/

	pthread_mutex_lock(&job_queue_mutex);

	/*Place the new job at the end of the queue*/

	new_job->next = job_queue;
	job_queue = new_job;

	/* Post semaphore to indicate that anothrer job is available. If threads are blocked,
	 * waiting on the semaphore, one will become unblocked so it ca process the job*/

	sem_post(&job_queue_count);

	/*unlock the job queue mutex*/

	pthread_mutex_unlock(&job_queue_mutex);


}
int main(int argc, char const *argv[])
{
    int i;
        pthread_t threads[5];
        pthread_t job_creator_thread;
        pthread_create(&job_creator_thread, NULL,job_creator ,NULL);
        //create threads to do the work
        for (i=0;i<5;++i)
                pthread_create(&(threads[i]),NULL,thread_function,NULL);
        for(i=0;i<5;++i)
        {
                pthread_join(threads[i], NULL);
        }
        return 0;

}

