#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
void interupt_handler()
{
    printf("SIGXCPU has been invoked \n");
}
int init_signal(int signal_no, struct sigaction *sig, void *handler)
{
	memset(sig, 0, sizeof(sig));
	sig->sa_handler = handler;
	sigaction(signal_no, sig, NULL);
}
int main()
{
    struct sigaction  sig_int;

    unsigned set_cpu_limit = 1, soft_limit, hard_limit;
	init_signal(SIGXCPU, &sig_int, &interupt_handler);
    struct rlimit rl;
    
    /* Obtain the current limits. */
    getrlimit(RLIMIT_CPU, &rl);
    /* Set a CPU limit of 1 second. */
    soft_limit = rl.rlim_cur;
    hard_limit = rl.rlim_max;
    printf("Current soft limit is %d and will be set to %d \n", soft_limit, set_cpu_limit);
    printf("Current hard limit is %d \n", hard_limit);
    rl.rlim_cur = set_cpu_limit;
    setrlimit(RLIMIT_CPU, &rl);
    /* Do busy work. */
   
    while (1)
        ;
    return 0;
}