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
    int sockfd,newsock;
    struct sockaddr_in address;
    socklen_t len=sizeof(address);
    char buffer[1024];
    int bytes;
    int ackprob=70;

    srand(time(0));

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family=AF_INET;
    address.sin_port=htons(port);
    address.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    listen(sockfd,4);

    printf("server is waiting for the connection.......\n");
    newsock=accept(sockfd,(struct sockaddr*)&address,&len);

    printf("Server connected......\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        bytes = read(newsock,buffer,sizeof(buffer));

        if(bytes<0)
        {
            printf("Server:- client disconnected...\n");
            break;
        }

        if(strncmp(buffer,"exit",4)==0)
        {
            printf("Server:client ends communication....\n");
            break;
        }

        printf("Server:-packet %s recieved\n",buffer);

        if(rand()%100<ackprob)
        {
            printf("Server:Ack for %s packet send\n",buffer);
            send(newsock,"ACK",strlen("ACK"),0);
        }
        else
        {
            printf("Server:packet lost for %s frame\n",buffer);

            // receive retransmission
            memset(buffer,0,sizeof(buffer));
            bytes = read(newsock,buffer,sizeof(buffer));

            printf("Server:-packet %s recieved\n",buffer);

            printf("Server:Ack for %s packet send\n",buffer);
            send(newsock,"ACK",strlen("ACK"),0);
        }
    }

    close(sockfd);
    close(newsock);
}