#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define port 8090

void main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    char file[1024];
    int byte;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);

    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Server connected.....\n");

    printf("Enter the file name: ");
    scanf("%s",file);

    send(sockfd,file,strlen(file),0);
    printf("File content:-\n");

    while((byte=recv(sockfd,buffer,sizeof(buffer),0))>0)
    {
        fwrite(buffer,1,byte,stdout);
    }
    close(sockfd);
}
