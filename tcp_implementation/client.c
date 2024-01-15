#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i,SYN=0,ACK=0;
	char buff[100];

	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}


	serv_addr.sin_family	= AF_INET;
	inet_aton("10.5.16.199", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(20000);

	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}

    char k[] = "2";

    send(sockfd,k,strlen(k)+1,0);

	printf("sent key = 2\n");

	int fd = open("file.txt",O_RDONLY);

	if(fd == -1){
        perror("Error opening file for reading\n");

        return 1;
    }

	char stream[2];
    ssize_t br;

	br = read(fd,stream,sizeof(stream)-1);

	// stream[strlen(stream)-1]='\0';

	// memset(buff,0,sizeof(buff));
	if(br == -1){
		perror("Error reading from the file");
		close(fd);
		return 1;
	}

	// stream[br] = '\0';

	printf("Read from file: %s \n",stream);
	send(sockfd,stream,strlen(stream)+1,0);
	printf("Sent '%s'\n",stream);

	// memset(buff,0,sizeof(buff));
	// strcpy(buff,"END");
	// send(sockfd,buff,strlen(buff)+1,0);

	close(fd);
	
		
	close(sockfd);
	return 0;

}