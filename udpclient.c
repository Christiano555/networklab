#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define port 8090
void main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t addrlen=sizeof(servaddr);
    char buffer[1024];
    char message[1024];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed");
        exit(1);
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    printf("UDP client started....\n");

    while(1)
    {
        printf("client:");
        fgets(message,sizeof(message),stdin);
        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&servaddr,addrlen);
        if(strncmp(message,"exit",4)==0) break;
        memset(buffer,0,sizeof(buffer));
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&servaddr,&addrlen);
        printf("server:%s",buffer);
        if(strncmp(buffer,"exit",4)==0) break;
    }
    close(sockfd);
}