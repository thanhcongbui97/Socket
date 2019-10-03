#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 1500
#define MAX_SIZE 10000

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

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	error("Couldn't create socket");

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	    error("Couldn't modify the socket"); 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons( PORT );     

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
    	error("Bind failed");

    if (listen(server_fd, 3) < 0) 
    	error("Linsten to client failed"); 
	
    if ( (client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, (socklen_t*)&addr_len )) < 0 ) 
	    error("Couldn't connect to the client");

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("ipclient: %s\n", client_ip );
    char cli_port[INET_ADDRSTRLEN];
    printf("port: %d\n", ntohs(cli_addr.sin_port));
        char buffer[1024];
    	if( read(client_fd, buffer, MAX_SIZE) < 0)
		error("Couldn't receive from client");
	char file_name[20];
     	system("ls");	
    	puts("Enter file name to save: ");
	gets(file_name);
	FILE *fp = fopen(file_name, "w+");
	char buffer[MAX_SIZE];

	fclose(fp);
    
	puts("Successfully received");
	system("ls");

    close(client_fd);
    return 0; 
} 
