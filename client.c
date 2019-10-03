#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#define PORT 1500
#define MESS_SIZE 50

int sock; 
void error(char *msg)
{
	perror(msg);
	exit(1);
}

void *receive(void *varg)
{
	char *msg = (char*)malloc(MESS_SIZE*sizeof(char));
	while(1)
	{
	bzero(msg,MESS_SIZE);
	if(read(sock, msg, MESS_SIZE) > 0)
	printf("\nMessage: %s\n",msg);
	}
}

void *send_to(void *varg)
{
	char *msg = (char*)malloc(MESS_SIZE*sizeof(char));
	while(1)
	{
	printf("Enter message to send: ");
	do{
		gets(msg);
		if(0 == strlen(msg))
			printf("\nEnter message to send: ");
		}while(	0 >= strlen(msg));
		write(sock, msg, strlen(msg));
		if(strcmp(msg, "bye") == 0)
			exit(1);
	}
	free(msg);
}


int main(int argc, char const *argv[]) 
{  
    struct sockaddr_in serv_addr; 
    char add[] = "10.72.0.10";
	time_t rawtime;
	struct tm *info;
/*
    printf("Nhap dia chi server\n");
    gets(add);
*/
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		error("Couldn't create socket");

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, add, &serv_addr.sin_addr) <= 0)  
        error("Invalid address/ Address not supported ");

	time( &rawtime );
	info = localtime( &rawtime );
	printf("Time start connecting: %s\n",asctime(info));
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		time(&rawtime);
		info = localtime( &rawtime );
		printf("Time end connecting: %s\n",asctime(info));
        perror("Connection failed"); 
	}
	pthread_t THREAD_1, THREAD_2;
	pthread_create(&THREAD_1, NULL, receive, NULL);
	pthread_create(&THREAD_2, NULL, send_to,  NULL);
	pthread_join(THREAD_1, NULL);
	pthread_join(THREAD_2, NULL);
   return 0; 
} 
