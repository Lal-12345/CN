
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 1024
#define NAME_SIZE 30

int main()
{
    int sock;
    struct sockaddr_in server;

    char name[NAME_SIZE]={0};
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    // Create socket
    sock=socket(AF_INET,SOCK_STREAM,0);

    // Configure server address
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    // Connect to server
    connect(sock,(struct sockaddr *)&server,sizeof(server));

    // Enter username
    printf("Enter your name: ");
    fgets(name,sizeof(name),stdin);

    name[strcspn(name,"\n")]='\0';

    send(sock,name,strlen(name)+1,0);

    printf("Connected to Chat Server\n");
    printf("Start chatting...\n");

    if(fork()==0)
    {
        // Child process: Receive messages
        int n;

        while((n=recv(sock,buffer,sizeof(buffer)-1,0))>0)
        {
            buffer[n]='\0';
            printf("%s",buffer);
            fflush(stdout);
        }

        exit(0);
    }
    else
    {
        // Parent process: Send messages
        while(1)
        {
            if(fgets(message,sizeof(message),stdin)==NULL)
                break;

            if(strcmp(message,"/exit\n")==0)
                break;

            send(sock,message,strlen(message),0);
        }

        shutdown(sock,SHUT_RDWR);
        close(sock);
    }

    return 0;
}

