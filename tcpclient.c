#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

void main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024],message[1024];
    #define port 8090

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed");
        exit(1);
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Client connected........\n");
    while(1)
    {
        printf("Client:- ");
        fgets(message,sizeof(message),stdin);
        send(sockfd,message,strlen(message),0);
        if(strncmp(message,"exit",4)==0)
        {
            break;
        }
        memset(buffer,0,sizeof(buffer));
        read(sockfd,buffer,sizeof(buffer));
        printf("Server:- %s",buffer);
        if(strncmp(buffer,"exit",4)==0)
            break;
    }
    close(sockfd);
}