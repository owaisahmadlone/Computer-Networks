#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i,SYN=0,ACK=0;
	char buff[100];		
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket\n");
		exit(0);
	}

	
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(20000);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5);

    printf("Server Running...\n");
    
    while(1){
        clilen = sizeof(cli_addr);

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if(newsockfd < 0){
            printf("Accept Error\n");
            exit(0);
        }

        if(fork() == 0){
            close(sockfd);

            recv(newsockfd, buff, 100, 0);

            
            int key = atoi(buff);

            printf("Received key : %d\n",key);

            memset(buff,0,sizeof(buff));
                
            recv(newsockfd,buff,100,0);

            printf("Client: %s\n",buff);


            int fd1 = open("server.txt",O_WRONLY | O_CREAT | O_APPEND);
            // int fd_enc = open("10.5.16.199.20000.txt.enc",O_WRONLY | O_CREAT | O_APPEND);

            if(fd1 == -1){
                perror("Error opening file for writing\n");
                return 1;
            }

            // if(strcmp(buff,"END") == 0) break;

            // char c = buff[0],newc;

            // key = key%26;

            // if((c+key) > 'z') newc = (c-(26-key));
            // else newc = c+key;

            // char stream[3];
            // stream[0]=newc;
            // stream[1]='\n';
            // stream[2]='\0';

            // buff[strlen(buff)-1] = '\n';

            ssize_t br = write(fd1,buff,sizeof(buff)-1);

            if(br == -1){
                perror("Error writing to the file");
                close(fd1);
                return 1;
            }

            close(fd1);

            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }

    return 0;
}