
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char filename[100], buffer[2048], line[200];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    printf("Server Started\n");

    while(1)
    {
        // Receive filename
        recvfrom(sockfd, filename, sizeof(filename), 0,
                 (struct sockaddr *)&client, &len);

        // Create child process
        if(fork() == 0)
        {
            FILE *fp;

            // Add PID
            sprintf(buffer, "PID: %d\n", getpid());

            // Open file
            fp = fopen(filename, "r");

            if(fp == NULL)
                strcat(buffer, "File Not Found");
            else
            {
                while(fgets(line, sizeof(line), fp))
                    strcat(buffer, line);

                fclose(fp);
            }

            // Send response
            sendto(sockfd, buffer, strlen(buffer)+1, 0,
                   (struct sockaddr *)&client, len);

            exit(0);
        }
    }

    close(sockfd);
    return 0;
}

