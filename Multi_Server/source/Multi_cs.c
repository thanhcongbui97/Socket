#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 1500
#define MESS_SIZE 50

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int fd_client[10] ;
int count = 0;

int main(int argc, char const *argv[]) 
{ 
    int server_fd, client_fd, valread; 
    struct sockaddr_in serv_addr, cli_addr; 
    int opt = 1;   
    int continu = 1;
    int addr_len = sizeof(serv_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	error("Couldn't create socket");

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ) 
	    error("Couldn't modify the socket"); 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons( PORT );     

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
    	error("Bind failed");
	
    if (listen(server_fd, 3) < 0) 
    	error("Linsten to client failed");
		printf("Opened connection at PORT %d\n",PORT);

    while(1)
    {
		
    	if( (client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t*)&addr_len )) < 0 )
			if(client_fd < 0)
				count--;
	    	else
				error("Couldn't connect to the client");


		count++;
	    char client_ip[INET_ADDRSTRLEN];
	    inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
		printf("Connected to client at address %s via port %d \n", client_ip, ntohs(cli_addr.sin_port));
		printf("Number of visitor: %d\n",count);
		FILE *fp = fopen("table.txt","a+");
		fprintf(fp, "%d\n", client_fd);
		fclose(fp);
		
    		if(fork() == 0)
            {
                close(server_fd);

                while(1)
				{	    
	    		char *buffer = (char*)calloc(MESS_SIZE, sizeof(char));

	  	  	    
	    		if(valread = recv(client_fd, buffer, MESS_SIZE, 0) < 0)
		    		error("Couldn't read from client");
	    		
				printf("Message from %s with port %d : %s\n",client_ip,ntohs(cli_addr.sin_port),buffer);

	    		send(client_fd, buffer, strlen(buffer), 0); 

	    		if(strncmp(buffer, "bye", 3) == 0)
				{
					printf("%s:%d has left\n",client_ip, ntohs(cli_addr.sin_port));
					break;
				}
				
	   	 	    free(buffer); 
				buffer =  '\0';
                }
    		}		
	}
   	close(client_fd);
    return 0; 
} 
