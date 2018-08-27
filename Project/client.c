

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD */
typedef struct thread_arg_t
{
        int id;
        char * socket_address;

};
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
void * thread_function(void * arg)
{
    struct thread_arg_t *socket_name = (struct thread_arg_t *)arg;
    int socket_fd;
    struct sockaddr_un name;
    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    /* Store the server’s name in the socket address. */
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name->socket_address);
    /* Connect the socket. */
    connect(socket_fd, &name, SUN_LEN(&name));
    /* Write the current time to the socket to the socket. */
    
    char * current_time;
    int counter = 1;
    char message[100];
    while (counter<185)
    {
    	current_time = get_timestamp();
        sprintf(message, "%d%d", (int)socket_name->id, counter);
        write_text(socket_fd, message);
	//close(socket_fd);
	sleep(1);
	//connect(socket_fd, &name, SUN_LEN(&name));
	//printf("reconnected \n");
	counter++;
    }
    printf("Exiting \n");
    write_text(socket_fd, "quit");

    close(socket_fd);
    return 0;
}
int main(int argc, char *const argv[])
{
        int i;
        const int no_threads = 5;
        pthread_t threads[no_threads];
        struct thread_arg_t thread_args[no_threads];
        for (i=0;i<no_threads;++i)
        {
                thread_args[i].socket_address = argv[1];
                thread_args[i].id = i;
                pthread_create(&(threads[i]),NULL,thread_function,(void *)&(thread_args[i]));
                sleep(1);
        }
                
        for(i=0;i<no_threads;++i)
        {
                pthread_join(threads[i], NULL);
        }
        return 0;
}