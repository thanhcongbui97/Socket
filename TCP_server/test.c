#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 1500
#define MESS_SIZE 20

void error(char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, client_fd, valread; 
    struct sockaddr_in serv_addr, cli_addr; 
    int opt = 1;   
    int continu = 1;
    int addr_len = sizeof(serv_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	error("Couldn't create socket");

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0 ) 
	    error("Couldn't modify the socket"); 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons( PORT );     

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
    	error("Bind failed");

		if(fork())
		{	
    if (listen(server_fd, 3) < 0) 
    	error("Linsten to client failed"); 
    		if( (client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t*)&addr_len )) < 0 ) 
	    	error("Couldn't connect to the client");

  		puts("ok");
	    	char client_ip[INET_ADDRSTRLEN];
	    	inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    		printf("ipclient: %s\n", client_ip );
    		char cli_port[INET_ADDRSTRLEN];
    		printf("port: %d\n", ntohs(cli_addr.sin_port));

    		while(continu == 1){   	    
	    		char *buffer = (char*)calloc(MESS_SIZE, sizeof(char));

	  	  	printf("Tin nhan ban nhan dc tu client: \n");
	    		if(valread = read( client_fd, buffer, MESS_SIZE) < 0)
		    	error("Couldn't read from client");
	    		printf("%s\n",buffer );

	    		write(client_fd, buffer, strlen(buffer)); 

	    		if(strncmp(buffer, "bye", 3) == 0)
		    	continu = 0;
	   	 	free(buffer);
    		}		
   		close(client_fd);
		}
	close(server_fd);
    return 0; 
} 
