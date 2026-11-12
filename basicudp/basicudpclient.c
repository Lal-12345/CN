#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 12345

int main()
{
    int sockfd;
    char buffer[1024];
    char message[1024];

    struct sockaddr_in server_addr;
    socklen_t len;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Enter message: ");
    fgets(message,sizeof(message),stdin);

    sendto(sockfd,message,strlen(message)+1,0,
           (struct sockaddr *)&server_addr,sizeof(server_addr));

    len=sizeof(server_addr);

    int n=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,
                   (struct sockaddr *)&server_addr,&len);

    buffer[n]='\0';

    printf("Server: %s\n",buffer);

    close(sockfd);

    return 0;
}
