
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


//Linux guarantees that assignments to variables of this type are performed in a single instruction
// and therefore cannot be interrupted midway. In Linux,

sig_atomic_t siguser1_count = 0;

void _print(char * message)
{
	printf("%s Received\n",message);
}
void handler ()
{
   ++siguser1_count;
}
void interupt_handler()
{


	_print("Interupt");
}
void quit_handler()
{
	_print("QUIT");
}
void stop_handler()
{
        _print("Stop");
}

int init_signal(int signal_no, struct sigaction *sig, void *handler)
{
	  memset(sig, 0, sizeof(sig));
	  sig->sa_handler = handler;
	  sigaction(signal_no, sig, NULL);

}	
int main()
{
	struct sigaction sa, sig_int, sig_quit, sig_stop;
	
	init_signal(SIGINT, &sig_int,&interupt_handler);
	init_signal(SIGSTOP, &sig_stop, &stop_handler);
	init_signal(SIGQUIT, &sig_quit, &quit_handler);

	/* main program run here*/

	while(1)
	{
	}

	printf("SIGUSR1 was raised %d times \n", siguser1_count);
	return 0;

}
