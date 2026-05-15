#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024],message[1024];
    int bytes;
    #define port 8090

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);

    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Client connected......\n");

    printf("Enter the file name:- ");
    scanf("%s",buffer);
    send(sockfd,buffer,strlen(buffer),0);
    printf("File content:--\n");

    while((bytes=recv(sockfd,message,sizeof(message),0))>0)
    {
        fwrite(message,1,bytes,stdout);
    }
    close(sockfd);
}