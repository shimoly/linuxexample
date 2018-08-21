#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <time.h>


/* Write TEXT to the socket given by file descriptor SOCKET_FD */
char *get_timestamp()
{
        time_t now = time(NULL);
        return asctime(localtime(&now));
}

void write_text(int socket_fd, const char *text)
{
    /* Write the number of bytes in the string, including NUL-termination. */
    int length = strlen(text) + 1;

    write(socket_fd, &length, sizeof(length));   
    write(socket_fd, text, length);
    printf("Write the string. %s (%d)\n", text, length);

}
int main(int argc, char *const argv[])
{
    const char *const socket_name = argv[1];
    const char * const message = argv[2];
    int socket_fd;
    struct sockaddr_un name;
    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    /* Store the server’s name in the socket address. */
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    /* Connect the socket. */
    connect(socket_fd, &name, SUN_LEN(&name));
    /* Write the current time to the socket to the socket. */
    
    char * current_time;
    int counter = 1;
    while (counter<185)
    {
    	current_time = get_timestamp();
        write_text(socket_fd, message);//current_time);
	//close(socket_fd);
	sleep(1);
	//connect(socket_fd, &name, SUN_LEN(&name));
	printf("reconnected \n");
	counter++;
    }
    printf("Exiting \n");
    write_text(socket_fd, "quit");

    close(socket_fd);
    return 0;
}
