#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define SIZE 1024
  
int main() { 
    int sockfd, err; 
    struct sockaddr_in servaddr; 
    int n;
    socklen_t len; 
    char filename[100] = "file.txt"; 
    char buffer[SIZE];
  
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(8181); 
    err = inet_aton("10.5.16.199", &servaddr.sin_addr);
    if (err == 0) {
	   printf("Error in ip-conversion\n");
	   exit(EXIT_FAILURE);
    }
    
    // Request for the file with a specific name
    sendto(sockfd, (const char *)filename, strlen(filename), 0, 
			(const struct sockaddr *) &servaddr, sizeof(servaddr));
    
    printf("Filename sent to Server\n");
    
    // Obtain the message from the server
    n = recvfrom(sockfd, (char *)buffer, SIZE,0, (struct sockaddr *)&servaddr, &len);
    printf("message '%s' received from server\n",buffer); 
    
    // Request remaining words from the server
    if(strcmp(buffer,"HELLO")==0){
        printf("File is present at server!\n");
        
        // Open a local file and first write HELLO to it
        FILE* fptr = fopen("client_file.txt","w");
        fprintf(fptr,"%s\n",buffer);
        printf("word '%s' received from server\n",buffer);
        
        int i=1;
        while (1)
        {
            char s[10] = "WORD",c[2];
            sprintf(c,"%d",i);
            strcat(s,c);
            
            // Request for WORDi
            usleep(10000);
           sendto(sockfd, (const char *)s, strlen(s), 0, 
			(const struct sockaddr *) &servaddr, sizeof(servaddr));
           printf("'%s' requested from server\n",s);
           
           // Clear the buffer and receive the word into the buffer
           memset(buffer, 0, sizeof(buffer));
           n = recvfrom(sockfd, (char *)buffer, SIZE,0, (struct sockaddr *)&servaddr, &len);
           printf("word '%s' received from server\n",buffer);
           
           // Print the word into the file
           fprintf(fptr,"%s\n",buffer); 

           // Exit when 'END' is encountered
           if(strcmp(buffer,"END")==0 || i==100) break;
           i++;
        }
        
    }     
    else{
        printf("File %s not found\n",filename);
    }
    printf("Done\n");
    close(sockfd); 
    return 0; 
} 
