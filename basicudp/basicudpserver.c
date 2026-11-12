#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 12345

int main()
{
    int sockfd;
    char buffer[1024];
    char reply[1024];

    struct sockaddr_in server_addr,client_addr;
    socklen_t len;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));

    printf("UDP Server is running...\n");

    len=sizeof(client_addr);

    int n=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,
                   (struct sockaddr *)&client_addr,&len);

    buffer[n]='\0';

    printf("Client: %s\n",buffer);

    printf("Enter reply: ");
    fgets(reply,sizeof(reply),stdin);

    sendto(sockfd,reply,strlen(reply)+1,0,
           (struct sockaddr *)&client_addr,len);

    close(sockfd);

    return 0;
}
