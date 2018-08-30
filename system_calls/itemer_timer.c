#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
char *get_timestamp()
{
        time_t now = time(NULL);
        return asctime(localtime(&now));
}
void get_timestamp_diff()
{
    struct timeval start , end;
    static long diff_counter = 0;
    static struct timespec tstart={0,0}, tend={0,0};
    
    if(diff_counter >= 1)
    {
        clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("%.5f \n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    }
    diff_counter++;
    clock_gettime(CLOCK_MONOTONIC, &tstart);    
    

}
void timer_handler(int signum)
{
    char * current_time = get_timestamp();
    printf("%s\n", current_time);
}
int main()
{
    struct sigaction sa;
    struct itimerval timer;
    /* Install timer_handler as the signal handler for SIGVTALRM.*/
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &get_timestamp_diff;
    sigaction(SIGVTALRM, &sa, NULL);
    /* Configure the timer to expire after 1500 msec... */
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 500000;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;
    get_timestamp_diff();
    /* Start a virtual timer. It counts down whenever this process is executing. */
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    /* Do busy work.*/
    while (1)
        ;
}