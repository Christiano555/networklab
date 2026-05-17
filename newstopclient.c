#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define port 8090

void main()
{
    int sockfd;
    struct sockaddr_in address;
    char buffer[1024]={0},packet[1024];
    int bytes;
    struct timeval tv;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

    connect(sockfd,(struct sockaddr*)&address,sizeof(address));
    printf("Client connected.......\n");

    tv.tv_sec=3;
    tv.tv_usec=0;
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));

    while(1)
    {
        printf("Enter the packet:");
        scanf("%s",packet);

        send(sockfd,packet,strlen(packet),0);

        if(strncmp(packet,"exit",4)==0)
        {
            break;
        }

        memset(buffer,0,sizeof(buffer));
        bytes=read(sockfd,buffer,sizeof(buffer));

        if(bytes>0&&(strncmp(buffer,"ACK",3)==0))
        {
            printf("Client:-Ack recived for packet %s\n",packet);
        }
        else
        {
            printf("Timeout retransmitting the packet %s.....\n",packet);

            // retransmit once
            send(sockfd,packet,strlen(packet),0);

            memset(buffer,0,sizeof(buffer));
            bytes=read(sockfd,buffer,sizeof(buffer));

            if(bytes>0&&(strncmp(buffer,"ACK",3)==0))
            {
                printf("Client:-Ack recived after retransmission for packet %s\n",packet);
            }
        }
    }
    close(sockfd);
}