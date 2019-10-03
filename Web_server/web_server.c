#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX_SIZE 5999999

void error(const char *msg)
{
	perror(msg);
	exit(1);
}



int main(int argc , char *argv[])
{
	int sockfd, newsockfd, port_no, n = 4, fding;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t addr_len = sizeof(cli_addr);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("Error opening socket");

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

	port_no = 1234;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_no);

	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Binding Failed");

	if(listen(sockfd, 5) < 0)
		perror("Listening failed");
	
	printf("Opened connection via port %d\n",port_no);

	while(1)
	{
		if((newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len)) < 0)
			error("Couldn't connect to the client");
			
		if(!fork())
		{
			FILE *index;
			char *webpage = (char*)malloc(30000*sizeof(char));
			index = fopen("index.html","r");
			int i = 0;
       			char c;
			while((c = fgetc(index)) != EOF)
			{
				webpage[i] = c;
				i++;
			}
			fclose(index);
			close(sockfd);
			char *buff = (char*)calloc(2048 , sizeof(char));
			char *filename = (char*)calloc(2045 , sizeof(char));
			read(newsockfd, buff, 2047);
			printf("%s\n",buff);
		
			if( strncmp(buff, "GET",3) == 0 && isalpha(buff[5]) )	/*Means it trasfering a file*/
			{
				puts("ok");
				i = 5;
				int j=0;
				while( isspace(buff[i]) == 0) /*Until it encountered next space*/
				{
					filename[j] = buff[i];
					j++;
					i++;
				}
				puts(filename);
				fding = open(filename, O_RDONLY);
				sendfile(newsockfd, fding, NULL, MAX_SIZE);
				close(fding);
				free(filename);

			}
			else
			write(newsockfd, webpage, strlen(webpage) );
			close(newsockfd);
			exit(0)
				;
		}
		close(newsockfd);
	}
}
