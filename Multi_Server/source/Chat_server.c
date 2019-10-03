#include <stdio.h> 
#include <string.h>  
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <time.h> 
	

    
#define TRUE 1 
#define FALSE 0 
#define PORT 1500
#define MES_SIZE 512
	
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int  addrlen , client_socket[30] , max_clients = 30 , activity, i , valread , sd; 
	int max_sd,master_socket, new_socket; 
	struct sockaddr_in address;
	time_t rawtime;
   	struct tm *info;
	
		 

	/* Set of socket file descriptor */ 
	fd_set readfds;
	
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
	
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) < 0)
		error("Couldn't create socket");
			 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 )
		error("Couldn't modify the socket");
	
	/* Asssign address */ 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0 ) 
		error("Couldn't bind the address");

	printf("Listener on port %d... \n", PORT); 
		
	if (listen(master_socket, 10) < 0) 
		error("Couldn't listen to the server");
		 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
	{  
		FD_ZERO(&readfds); 
	
		/*add master socket to set */ 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket;
			
		/* add child sockets to set*/ 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			/*  socket descriptor  */
			sd = client_socket[i];
				
			/* if valid socket descriptor then add to read list */
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
		
		printf("%d", activity);	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			puts("select error");
		} 
			
		/* If something happened on the master socket , then its an incoming connection */ 
/*		if (FD_ISSET(master_socket, &readfds)) 
		{ 
*/			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
				error("Connection error");
			 
			printf("Connected to client with fd: %d at : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
		
			/* Welcome */
			char *welcome = "Welcome to my server\n";
			if( send(new_socket, welcome, strlen(welcome), 0) != strlen(welcome) ) 
				perror("send"); 
			
			puts("Welcome message sent successfully"); 
				

			for (i = 0; i < max_clients; i++) 
			{ 
				
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 		
					break; 
				} 
			} 
//		} 
			
		/* else its some IO operation on some other socket  */
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i];

			if (FD_ISSET( sd , &readfds)) 
			{ 
				/* Check if it was for closing , and also read the incoming message */
				puts("OK");
				char *buffer = (char*)malloc(MES_SIZE * sizeof(char));
				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					/* Somebody disconnected , get his details and print */ 
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				/*  Echo the message to other client */
				else
				{	
   					time( &rawtime );
   					info = localtime( &rawtime );
					char *moment = (char*)malloc(MES_SIZE* sizeof(char));
					strcpy(moment, asctime(info));
					int len = strlen(moment) - 1;
					moment[len] = '\t';

					buffer[valread] = '\0';
					strcat(moment, buffer);
					puts(moment);
					FILE *flog = fopen("chat_log.txt","a+");
					fprintf(flog, "%s\n", moment);
					fclose(flog);

					int j; 
					for (j = 0; j < max_clients; j++)
					{	
					/* 	if(client_socket[j] != sd)  */
						send(client_socket[j] , buffer , strlen(buffer) , 0 );
					}
					free(buffer);
					buffer = '\0';
					free(moment);
					moment = '\0';
				} 
			} 
		} 
	} 
		
	return 0; 
} 

