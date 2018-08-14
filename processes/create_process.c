#include <stdio.h>
#include <stdlib.h> // for system
#include <unistd.h> // for fork


int create_process_using_system()

{
	//ps -e -p pid,ppid,command returns id it's parent id the running command of the running process, 
        //using System to excute command is unreliable and has security proble
        int return_value = system("ps -e -o pid,ppid,command");
        return return_value;
}
int create_using_fork()
{
	pid_t child_pid;
	printf ("the main program process ID is %d\n", (int) getpid ());
	child_pid = fork ();
	if (child_pid != 0) 
	{
		printf ("this is the parent process, with id %d\n", (int) getpid ());
		printf ("the child’s process ID is %d\n", (int) child_pid);
	}
	else
		printf ("this is the child process, with id %d\n", (int) getpid ());
	return 0;
}
int create_new_process_using_fork_exec(char* program_name, char ** arg_list)
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
	
	create_new_process_using_fork_exec("ls", arg_list);
}

