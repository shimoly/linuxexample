
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
/* Read text from the socket and print it out. Continue until the socket closes. Return nonzero if the client sent a "quit" message, zero otherwise. */
int server(int client_socket)
{
    while (1)
    {
        int length;
        char *text;
	int quit_message = 0;
        /* First, read the length of the text message from the socket.read returns zero, the client closed the connection. */
        if (read(client_socket, &length, sizeof(length)) == 0)
	{
		printf("Read Error \n");
	        return 1;
	}
        /* Allocate a buffer to hold the text. */
        text = (char *)malloc(length);
        /* Read the text itself, and print it. */
        read(client_socket, text, length);
        quit_message = strcmp(text, "quit");

        printf("Received. (%d)\t%s\n",client_socket, text);
	
        /* Free the buffer. */
        free(text);
        /* If the client sent the message "quit," we’re all done.*/
	if (quit_message == 0)
	{
	 printf("Quiting\n");
	 return 1;
	}
	//return 0;
    }
    return 0;
}
int init_server(const char *const socket_name)
{
    //const char *const socket_name = argv[1];

    int socket_fd;
    struct sockaddr_un name;
    int client_sent_quit_message;
    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    /* Indicate that this is a server. */
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    
    bind(socket_fd, &name, SUN_LEN(&name));
    /* Listen for connections. */
    listen(socket_fd, 5);
    /* Repeatedly accept connections, spinning off one server() to deal with each client. Continue until a client sends a "quit" message.*/
    do
    {
        struct sockaddr_un client_name;
        socklen_t client_name_len;
        int client_socket_fd;

        /* Accept a connection. */
        client_socket_fd = accept(socket_fd, &client_name, &client_name_len);
        /* Handle the connection. */
       
        client_sent_quit_message = server(client_socket_fd);
        /* Close our end of the connection. */
        close(client_socket_fd);
    } while (!client_sent_quit_message);
    /* Remove the socket file.*/

    printf("Server exiting \n");
    close(socket_fd);
    unlink(socket_name);

    return 0;
}

int main(int argc, char *argv[])
{
	const char *const socket_name = argv[1];
	int socket_fd;
        //	struct socketaddr_un name;

	struct stat st;
	int status = stat(socket_name, &st);
	if(status == 0)
	{
		/*A file already exits, check if it is a socket
		 * If yes unlink it
		 * If no treat it as an error condition*/
		if((st.st_mode & S_IFMT)==S_IFSOCK)
		{
		 status =unlink(socket_name);
	         if(status != 0)
		 {
			perror("Error unlinking the socket mode \n");
			exit(1);
		 }
		}
	
		else
		{
			fprintf(stderr, "The path exits but it is not a socket\n");
			exit(1);
		}
	}
	init_server(socket_name);


	

}


