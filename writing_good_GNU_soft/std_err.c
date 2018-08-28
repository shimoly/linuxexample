#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
enum messageType
{
	info = 1,
	errormsg
};
int print_info_error(enum messageType msgType)
{
	switch (msgType)
	{
	case info:
		fprintf(stdout, ".");
		break;
	case errormsg:
		fprintf(stderr, ".");
		break;
	default:
		printf("Error: Not valid choise Enter 1 for stdout and 2 for stderror \n");
		printf("These three streams are also accessible with the underlying UNIX I/O commands ( read , write , and so on) via file descriptors.These are file descriptors 0 for stdin , 1 for stdout , and 2 for stderr");
		return 1;
	}
	return 0;
}
int main(int argc, char *argv[])
{
	int counter = 0;

	int choise = 0;
	if (argc > 1)
		choise = argv[1][0] - '0';
	while (counter < 120)
	{
		if (print_info_error(choise))
			break;
		sleep(1);
		counter++;
	}
	printf("\n Program Exited \n");
	return 0;
}
