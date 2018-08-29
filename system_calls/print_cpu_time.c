#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>




void print_cpu_time()
{
struct rusage usage;
getrusage (RUSAGE_SELF, &usage);
printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n",
usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}
int main()
{
    int i = 0;
    while(i++<10)
        sleep(1);
    print_cpu_time();
}
