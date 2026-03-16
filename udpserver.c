#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define port 8090
int main()
{
    int sockfd;
    struct sockaddr_in servaddr,clienaddr;
    socklen_t addrlen=sizeof(clienaddr);
    char message[1024];
    char buffer[1024];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed....\n");
        exit(1);
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    servaddr.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("UDP server waiting for messages.....\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clienaddr,&addrlen);
        printf("Client:%s",buffer);
        if(strncmp(buffer,"exit",4)==0) break;
        printf("Server:");
        fgets(message,sizeof(message),stdin);
        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clienaddr,addrlen);
        if(strncmp(message,"exit",4)==0) break;
    }
    close(sockfd);
    return 0;
}