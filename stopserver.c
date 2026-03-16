#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define port 8090

void main()
{
    int servfd,newsocket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char buffer[1024];
    int ackprob =70;
    
    srand(time(0));

    servfd=socket(AF_INET,SOCK_STREAM,0);
    if (servfd<0)
    {
        perror("socket creation failed....\n");
        exit(1);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);

    bind(servfd,(struct sockaddr*)&address,sizeof(address));
    listen(servfd,4);
    printf("Server ready.....\n");

    newsocket=accept(servfd,(struct sockaddr*)&address,&addrlen);
    printf("server connected....\n");
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int valread=read(newsocket,buffer,sizeof(buffer));

        if(valread<=0)
        {
            printf("connection ended...\n");
            break;
        }

        if(strncmp(buffer,"exit",4)==0)
        {
            printf("Server:client ended communication\n");
            break;
        }

        printf("Server: packet %s recieved\n",buffer);

        if(rand()%100<ackprob)
        {
            printf("Server: ACK for packet %s sent\n\n",buffer);
            send(newsocket,"ACK",strlen("ACK"),0);
        }
        else
        {
             printf("Server: ACK for packet %s lost\n\n",buffer);           
        }

    }
    close(servfd);
    close(newsocket);
}