#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 1500

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
    int serverSocket;
    if( socket(PF_INET,SOCK_STREAM,0) < 0)
        error("Couldn't create socket");
    
    // Set reuse option
    int i = 1;
    if( setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int)) < 0)
        error("Could't modify the socket");

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( PORT );
    serverAddress.sin_addr.s_addr = INADDR_ANY;

	socklen_t len = sizeof(struct sockaddr_in);

	if( bind(serverSocket,(struct sockaddr*)&serverAddress,len) < 0)
	    error("Could't bind to the socket");

        
    if( listen(serverSocket,10) < 0)
        error("Could't listen to the server");


    struct sockaddr_in clientAddress;
        len = sizeof(struct sockaddr_in);
        fd_set readfds;
        fd_set masterfds;  /* The masterfds to record the changes of readfds */
        FD_ZERO(&masterfds);
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&masterfds); /*  Adding server Socket to master fds */
        
        int max_fd = serverSocket;  /* Get the highest number of socket */
        struct timeval timeout;     /* Set the timeout for connection */
        timeout.tv_sec = 90;
        timeout.tv_usec = 0;
        int n_select;
    do{
        memcpy(&readfds,&masterfds,sizeof(masterfds));
        n_select = select(max_fd + 1, &readfds,NULL,NULL,NULL);
        if (n_select < 0)
            error("Select error");

        else if (n_select == 0)
            puts("Connection timeout");

        else
        {
            /* Traversal the fd list */
            for (i = 0; i <= max_fd;i++)
                {
                    if (FD_ISSET(i,&readfds))
                    {
                    	int close_fd = 0;
                        if (i == serverSocket)
                        {
                            int newCon = accept(serverSocket,(struct sockaddr*)&clientAddress,&len);
                            printf("New connection \n");

                             FD_SET(newCon,&masterfds);
                            if (newCon > max_fd ) 
                            max_fd = newCon;
                        }
                        else
                        {
                            char message[100];
                            printf("Message from socket %d\n",i);
                            int nrecv = recv(i,message,100,0);
                            if (nrecv == -1)
                            {
                                printf("In socket %d\n",i);
                                perror("RECEIVE");
                                close_fd = 1;
                            }
                            else
                            if (nrecv != 0)
                                    {
                                        message[nrecv] =  '\0';
                                        char *hello = "Welcome to my server";
                                        printf("Client say : %s\n", hello);
                                        int j, nsent;
                                        for(j = 0; j<= max_fd; j++)
                                            {
                                            nsent = send(j,hello,strlen(hello),0);
                                            if (nsent == -1)
                                            {
                                                printf("In socket %d\n",i);
                                                perror("SEND");
                                                close_fd = 1;
                                            }
                                        }
                                    }
                                    else
                                    {
                                    	close_fd = 1;
                                    }
                        }
                        if (close_fd == 1) 
                        {
                        	FD_CLR(i,&masterfds);
                        	close(i);
                        }
                    }
                }
        }
    }
    while(1);
    return 0;
}
