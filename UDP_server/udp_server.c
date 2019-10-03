#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

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
	char buffer[MESS_SIZE] = {0};	
	char message[] = "Hello from server";
	
   	if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
		error("Couldn't create socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( PORT );     

    	if (bind(server_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    		error("Bind failed");

	printf("Opened connection on port %d...\n",PORT);
	int continu = 1;
	socklen_t len;
	while(continu == 1)
	{
		char addr[INET_ADDRSTRLEN];
		struct sockaddr_in cli_addr;

		recvfrom(server_fd, buffer, MESS_SIZE, MSG_WAITALL, ( struct sockaddr*)&cli_addr, &len);  

		inet_ntop(AF_INET, &cli_addr.sin_addr, addr, INET_ADDRSTRLEN);
		
		printf("\nIP: %s\tPort: %d\n", addr,ntohs(cli_addr.sin_port));
		printf("Client : %s\n", buffer); 
    		
		sendto(server_fd, message, strlen(message), MSG_CONFIRM, (struct sockaddr*)&cli_addr,len);
	
	 	if(strncmp(message, "bye" , 3) == 0)
       
			continu = 0;
      
	       	else      
   			printf("Hello message sent.\n");
      
	       	bzero(buffer, MESS_SIZE);
		bzero(message,MESS_SIZE);
	}
/*
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons( PORT +1);
	
	char addr[] = "10.72.110.50";
	inet_pton(AF_INET, addr,(struct sockaddr*) &cli_addr.sin_addr);

	sendto(server_fd, message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&cli_addr,
			       	sizeof(cli_addr));
*/  	return 0; 
} 
