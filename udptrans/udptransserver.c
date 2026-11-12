
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 12345

int main()
{
    int sockfd;
    char buffer[1024], result[1024]="", temp[100];

    char *abbr[] = {
        "tbh", "ig", "tbf", "atm", "irl", "lol",
        "asap", "omg", "ttyl", "idk", "nvm", "idc"
    };

    char *full[] = {
        "to be honest", "I guess", "to be fair",
        "at the moment", "in real life", "laughing out loud",
        "as soon as possible", "oh my God",
        "talk to you later", "I don't know",
        "never mind", "I don't care"
    };

    int i, j, found;

    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("UDP Server running...\n");

    len = sizeof(client_addr);

    int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                     (struct sockaddr *)&client_addr, &len);

    buffer[n] = '\0';

    printf("Received: %s", buffer);

    i = 0;

    while(sscanf(buffer+i, "%99s", temp) == 1)
    {
        found = 0;

        for(j = 0; j < 12; j++)
        {
            if(strcmp(temp, abbr[j]) == 0)
            {
                strcat(result, full[j]);
                found = 1;
                break;
            }
        }

        if(found == 0)
        {
            strcat(result, temp);
        }

        strcat(result, " ");

        i += strcspn(buffer+i, " \t\n");
        i += strspn(buffer+i, " \t\n");
    }

    strcpy(buffer, result);

    sendto(sockfd, buffer, strlen(buffer)+1, 0,
           (struct sockaddr *)&client_addr, len);

    printf("\nTranslated: %s\n", buffer);

    close(sockfd);

    return 0;
}

