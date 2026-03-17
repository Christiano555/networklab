#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define port 8090

void main()
{
    int servfd,newsocket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    int lossprob=30;
    int ack;
    char buffer[1024];

    srand(time(0));
    servfd=socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(servfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));

    address.sin_family=AF_INET;
    address.sin_port=htons(port);
    address.sin_addr.s_addr=INADDR_ANY;

    bind(servfd,(struct sockaddr*)&address,sizeof(address));
    listen(servfd,4);
    printf("Server waiting for connection......\n");

    newsocket=accept(servfd,(struct sockaddr*)&address,&addrlen);
    printf("Server: client connected....\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int valread=read(newsocket,buffer,sizeof(buffer));
        if (valread<=0)
        {
            printf("Server:client finished or disconnected\n");
            break;
        }
        ack=atoi(buffer);
        printf("Server: recieved packet %d\n",ack);
        if(rand()%100<lossprob)
        {
            printf("Server:ACK for packet %d lost\n",ack);
        }
        else
        {
            sleep(1);
            printf("Server:ACK for packet %d sent\n",ack);
            memset(buffer,0,sizeof(buffer));
            sprintf(buffer,"%d",ack);
            send(newsocket,buffer,strlen(buffer)+1,0);
        }

    }
    close(newsocket);
    close(servfd);
}