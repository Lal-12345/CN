
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;
    char buffer[100];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    strcpy(buffer, "TIME");

    sendto(sockfd, buffer, strlen(buffer)+1, 0,
           (struct sockaddr *)&server, len);

    recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
             NULL, NULL);

    printf("\nCurrent Server Time:\n");
    printf("%s\n", buffer);

    close(sockfd);

    return 0;
}

