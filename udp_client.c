#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 1500 
#define MAXLINE 1024 

void error( char *msg)
{
	perror(msg);
	exit(1);
}


// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 serv_addr, cli_addr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
		error("Coudln't create socket");

	cli_addr.sin_family = AF_INET; 
	cli_addr.sin_port = htons(PORT+1); 
	cli_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) < 0)
		error("Bind failded");

	// Filling server information 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	serv_addr.sin_addr.s_addr = INADDR_ANY;	
/*	char add[] = "10.72.112.50";
	if(inet_pton(AF_INET, add, &serv_addr.sin_addr) <= 0)
         error("Invalid address/ Address not supported ");
*/
	socklen_t len; 
	sendto(sockfd, hello, strlen(hello), MSG_CONFIRM, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	printf("Hello message sent.\n"); 
		
	recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serv_addr, &len); 
	printf("Server : %s\n", buffer); 

	close(sockfd); 
	return 0; 
}
