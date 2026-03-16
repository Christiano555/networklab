#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define port 8090
int main()
{
    int clientfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    char message[1024];

    clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0)
    {
        perror("socket creation failed...");
        exit(1);
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);

    connect(clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Client is connected.....\n");

    while(1)
    {
        printf("Client:");
        fgets(message,sizeof(message),stdin);
        send(clientfd,message,strlen(message),0);
        if(strncmp(message,"exit",4)==0) break;
        memset(buffer,0,sizeof(buffer));
        read(clientfd,buffer,sizeof(buffer));
        printf("Server:%s",buffer);
        if(strncmp(buffer,"exit",4)==0) break;
    }
    close(clientfd);
    return 0;
}