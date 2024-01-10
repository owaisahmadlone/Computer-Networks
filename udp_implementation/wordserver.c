#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#define SIZE 1024

int main(){
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int n;
    socklen_t len;
    char buffer[SIZE],string_stream[SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(8181); 

    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("bind failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Server Running!....\n");

    len = sizeof(cliaddr);
    
    n = recvfrom(sockfd, (char *)buffer, SIZE,0, (struct sockaddr *)&cliaddr, &len);
    
    printf("Request for file '%s'\n",buffer);


    if(access(buffer,F_OK) == -1){
        char file_error[100] = "NOT FOUND FILE\n";
        sendto(sockfd, (const char *)file_error, 100, 0, 
			(const struct sockaddr *) &cliaddr, sizeof(cliaddr));
    }
    else{
        FILE* file = fopen(buffer,"r");

        if(fgets(string_stream,sizeof(string_stream),file) == NULL){
            perror("Error reading HELLO from file!\n");
            exit(EXIT_FAILURE);
        }
        string_stream[strlen(string_stream)-1] = '\0';

        // Send HELLO to the client
        sendto(sockfd, string_stream, strlen(string_stream), 0, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));

        printf("Sent HELLO\n");
        

        //Send remaining words to the client

        while (fscanf(file, "%s", string_stream) == 1) {
            if(strcmp(string_stream,"HELLO") == 0 ) continue; 

            usleep(10000);
            
            memset(buffer, 0, sizeof(buffer));
            n = recvfrom(sockfd, (char *)buffer, SIZE,0, (struct sockaddr *)&cliaddr, &len);
            if(n == -1) break;
            printf("Request for '%s'\n",buffer);
            
            
            sendto(sockfd, (char*) string_stream, strlen(string_stream), 0, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));
            printf("Sent word '%s'\n",string_stream);
        }
        
        printf("All words sent!\n");
        fclose(file);  
    }
    // close(sockfd);
    return 0;
}