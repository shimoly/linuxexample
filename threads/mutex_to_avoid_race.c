#include <malloc.h>
#include <pthread.h>
struct job {
/* Link field for linked list.*/
struct job* next;

/* Other fields describing work to be done... */
};
/* A linked list of pending jobs.*/
struct job* job_queue;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;


void process_job(struct job* current_job)
{
    if(current_job == NULL)
        printf("Current job is null\n");
    else
        printf("%p\n",current_job);
}
/* Process queued jobs until the queue is empty.*/
void* thread_function (void* arg)
{
    
    while (1) 
    {
        struct job* next_job;
        /* Lock the mutex on the job queue. */

        pthread_mutex_lock (&job_queue_mutex);
        /* Now it’s safe to check if the queue is empty. */
        if (job_queue == NULL)
            next_job = NULL;
        else 
        {
            /* Get the next available job. */
            next_job = job_queue;
            /* Remove this job from the list. */
            job_queue = job_queue->next;
        }
        /* Unlock the mutex on the job queue because we’re done with the queue for now. */
        pthread_mutex_unlock (&job_queue_mutex);
        /* Was the queue empty? If so, end the thread.*/
        if (next_job == NULL)
        {
            
            printf("Exiting .... %d\n", (int)pthread_self());
            break;
        }
            
        
        
        /* Carry out the work. */
        process_job (next_job);
        /* Clean up. */
        free (next_job);
    }
    return NULL;
}
int add_job()
{
    struct job *next_job = job_queue;
    struct job *temp_job = job_queue;
   
    while(next_job != NULL)
    {
        temp_job = next_job->next;
        next_job = temp_job;
    }
    struct job next_avail_job;
    next_job = &next_avail_job;
    printf("Starting .... %d, %p\n", (int)pthread_self(), job_queue);

}
int main(int argc, char const *argv[])
{
    int i;
	pthread_t threads[5];

	for (i=0;i<5;++i)
		add_job();
	//create threads to do the work
	for (i=0;i<5;++i)
		pthread_create(&(threads[i]),NULL,thread_function,NULL);
	for(i=0;i<5;++i)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
    
}

