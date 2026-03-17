#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>

#define port 8090
#define timestop 3

void main()
{
    int sockfd=0;
    struct sockaddr_in servaddr;
    int ack;
    char buffer[1024];
    struct timeval tv;

    int window,packet;
    printf("Enter window size:");
    scanf("%d",&window);

    printf("Enter no.of packets:");
    scanf("%d",&packet);

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);

    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Client: server connected.....\n");

    tv.tv_sec=timestop;
    tv.tv_usec=0;
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));

    int base=1;
    int nextsend=1;
    int getack=0;

    while(getack<packet)
    {
        while(nextsend<base+window&&nextsend<=packet)
        {
            printf("Client: packet %d sent\n",nextsend);
            memset(buffer,0,sizeof(buffer));
            sprintf(buffer,"%d",nextsend);
            send(sockfd,buffer,strlen(buffer),0);
            nextsend++;
        }
        memset(buffer,0,sizeof(buffer));
        int valread=read(sockfd,buffer,sizeof(buffer));
        if (valread>0)
        {
            ack=atoi(buffer);
            printf("Client:ACK of %d packed recieved\n",ack);
            if(ack==base)
            {
                base=ack+1;
                getack=ack;
            }
        }
        else
        {
            printf("Client:Timeout......retransmitting from packet %d",base);
            nextsend=base;
        }
    }
    close(sockfd);
}