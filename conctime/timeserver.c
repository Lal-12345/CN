
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 9000

int main()
{
    int sockfd;
    char buffer[100];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("UDP Time Server Started\n");

    while(1)
    {
        len = sizeof(client);

        recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                 (struct sockaddr *)&client, &len);

        time_t currentTime;

        time(&currentTime);

        char *timeString = ctime(&currentTime);

        sendto(sockfd, timeString, strlen(timeString)+1, 0,
               (struct sockaddr *)&client, len);

        printf("Time request served.\n");
    }

    close(sockfd);

    return 0;
}

