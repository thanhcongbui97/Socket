#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 1500 
#define MAX_SIZE 100000
void error(char *msg)
{
	perror(msg);
	exit(1);
}



int main(int argc, char const *argv[]) 
{  
    int serv_fd, client_fd,opt; 
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t addr_len = sizeof(serv_addr); 

    	if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		error("Couldn't create socket");

	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(PORT);
    	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	
	if(setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		error("Couldn't modify the socket");

	if(bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		error("Couldn't bind the address");

	if(listen(serv_fd, 3) < 0)
		error("Couldn't listening to client");

	if(client_fd = accept(serv_fd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len) < 0)
		error("Connection failed");

	int fp;
	char file_name[20];
	system("ls");
	puts("Enter file name to send: ");
	gets(file_name);
	fp = open(file_name, O_RDONLY);
	sendfile(client_fd, fp, NULL, MAX_SIZE);
	close(fp);	
	close(serv_fd);
    return 0; 
}
