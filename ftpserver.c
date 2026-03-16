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
    int servfd,newsocket;
    struct sockaddr_in address;
    socklen_t addrlen= sizeof(address);
    char buffer[1024];
    char filename[1024];
    FILE *file;
    int byte;

    servfd=socket(AF_INET,SOCK_STREAM,0);

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);

    bind(servfd,(struct sockaddr*)&address,sizeof(address));
    listen(servfd,4);
    printf("Server waiting for the connection....\n");

    newsocket=accept(servfd,(struct sockaddr*)&address,&addrlen);
    printf("Client connected...\n");

    memset(buffer,0,sizeof(buffer));
    recv(newsocket,buffer,sizeof(buffer),0);
    printf("Client requested for file %s\n",buffer);

    file=fopen(buffer,"r");

    if(file==NULL)
    {
        strcpy(buffer,"File not found");
        send(newsocket,buffer,sizeof(buffer),0);
    }

    while((byte=fread(buffer,1,sizeof(buffer),file))>0)
    {
        send(newsocket,buffer,byte,0);
    }

    fclose(file);
    close(newsocket);
    close(servfd);
}