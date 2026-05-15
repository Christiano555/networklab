#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void main()
{
    int sockfd,newsock;
    struct sockaddr_in address;
    socklen_t len=sizeof(address); 
    FILE *file;
    int bytes;
    char buffer[1024],message[1024];
    #define port 8090

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed\n");
        exit(1);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,3);
    printf("Server waiting for connection...\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&len);
    printf("Server connected......\n");
    memset(buffer,0,sizeof(buffer));
    recv(newsock,buffer,sizeof(buffer),0);
    file=fopen(buffer,"r");
    printf("Client requested file is %s\n",buffer);
    file=fopen(buffer,"r");
    if(file==NULL)
    {
        printf("Unable to locate %s\n",buffer);
        strcpy(buffer,"file not found!!!");
        send(newsock,buffer,strlen(buffer),0);
        return;
    }

    while((bytes=fread(message,1,sizeof(message),file))>0)
    {
        send(newsock,message,bytes,0);
    }

    printf("server executed successfully..........\n");
    
    fclose(file);
    close(newsock);
    close(sockfd);
}