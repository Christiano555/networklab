#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

void main()
{
    int sockfd,newsocket;
    struct sockaddr_in address;
    socklen_t len=sizeof(address);
    char buffer[1024];
    char message[1024];

    #define port 8090

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("Socket creation failed\n");
        exit(1);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);

    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);

    printf("Server waiting for connection....\n");
    newsocket=accept(sockfd,(struct sockaddr*)&address,&len);
    printf("Client connected....\n");
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        read(newsocket,buffer,sizeof(buffer));
        printf("Client:%s",buffer);
        if(strncmp(buffer,"exit",4)==0)
            break;
        printf("Server:");
        fgets(message,sizeof(message),stdin);
        send(newsocket,message,strlen(message),0);
        if(strncmp(message,"exit",4)==0)
            break;
    }

    close(newsocket);
    close(sockfd);
}
