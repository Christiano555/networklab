#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

void main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024],message[1024];
    socklen_t len=sizeof(servaddr);
    #define port 8090

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("socket creation failed....\n");
        exit(1);
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    printf("Client ready for UDP communications....\n");

    while(1)
    {
        printf("Client:- ");
        fgets(message,sizeof(message),stdin);
        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&servaddr,len);
        if(strncmp(message,"exit",4)==0)
        {
            break;
        }
        memset(buffer,0,sizeof(buffer));
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&servaddr,&len);
        printf("Server:- %s",buffer);
        if(strncmp(buffer,"exit",4)==0)
        {
            break;
        }
    }
    close(sockfd);
}