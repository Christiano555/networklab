#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define port 8090

void main()
{
    int sockfd;
    struct sockaddr_in clientaddr;
    char buffer[1024];
    char packet[50];
    struct timeval tv;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed....\n");
        exit(1);
    }
    
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(8090);
    inet_pton(AF_INET,"127.0.0.1",&clientaddr.sin_addr);

    connect(sockfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
    printf("client connected....\n");

    tv.tv_sec=3;
    tv.tv_usec=0;

    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));


    while(1)
    {
        printf("Enter the packet:");
        fgets(packet,sizeof(packet),stdin);
        send(sockfd,packet,strlen(packet),0);

        if(strncmp(packet,"exit",4)==0) break;

        memset(buffer,0,sizeof(buffer));
        int valread=read(sockfd,buffer,sizeof(buffer));

        if(valread>0 && strncmp(buffer,"ACK",3)==0)
        {
            printf("Client:ACK for packet %s recieved\n\n",packet);
        }
        else
        {
            printf("TIMEOUT.....Retransmit the packet\n\n");
        }

    }
    close(sockfd);
}