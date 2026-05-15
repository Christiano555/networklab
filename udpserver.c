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
    struct sockaddr_in servaddr,clientaddr;
    socklen_t len=sizeof(clientaddr);
    char message[1024],buffer[1024];
    #define port 8090

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        perror("Socket creation failed....\n");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_port=htons(port);

    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    printf("Server ready for udp communication...\n");
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len);
        printf("Client:- %s",buffer);
        if(strncmp(buffer,"exit",4)==0)
            break;
        printf("Server:- ");
        fgets(message,sizeof(message),stdin);
        sendto(sockfd,message,strlen(message),0,(struct sockaddr*)&clientaddr,len);
        if(strncmp(message,"exit",4)==0)
            break;
    }
    close(sockfd);
}
