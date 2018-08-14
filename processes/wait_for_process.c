#include <stdio.h>
#include <stdlib.h> // for system
#include <unistd.h> // for fork



int spawn(char* program_name, char ** arg_list)
{
	pid_t child_pid;
       /*Duplicate the process
	*Functions that contain the letter p in their names ( execvp and execlp ) accept a program name and
         search for a program by that name in the current execution path
	* functions that don’t contain the p must be given the full path of the program to be executed
	*
	* Functions that contain the letter v in their names ( execv , execvp , and execve ) 
	* accept the argument list for the new program as a NULL-terminated array of pointers to strings
	*
	* Functions that contain the letter l ( execl , execlp , and execle ) accept the argument list 
	* using the C language’s varargs mechanism.
	* 
	*Functions that contain the letter e in their names ( execve and execle ) accept an
	additional argument, an array of environment variables.The argument should be 
	a NULL-terminated array of pointers to character strings. Each character string
	should be of the form “ VARIABLE=value ”
	* */	

	child_pid = fork();
	if(child_pid != 0)
	{
		return child_pid;
	}
	else
	{
		/*Now excute the program, search for it in the path*/
		execvp(program_name, arg_list);

		// the execvp returns only if an eroor occurs
		fprintf(stderr, "An error has occured in exccvp");
		abort();

	}
}
int main()
{
  	//create_process_using_system();
	//create_using_fork();
	char * arg_list[] =
       	{
		"ls",
		"-l",
		"/",
		NULL
	};
	int child_status;
	spawn("ls", arg_list);

	//wait for the child process to complete
	wait(&child_status);

	//waitpid() can be used to wait for a particular child process
	//The wait3 function returns CPU usage statistics about the exiting child process
	//wait4 allows you to wait specify additional options

	if(WIFEXITED(child_status))
		printf("The child process exited normally with the exit code %d \n",WEXITSTATUS(child_status));
	else
		printf("Exited abnormally \n");

	//If the program died from an unhandled signal. the WTERMSIG macro to extract from its exit status the signal number by which it died.

}

