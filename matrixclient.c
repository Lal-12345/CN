
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 12345

int main()
{
    int new_socket;
    struct sockaddr_in address;
    int N,matrix[10][10];
    char result[100];

    new_socket=socket(AF_INET,SOCK_STREAM,0);

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(PORT);

    printf("Enter N: ");
    scanf("%d",&N);

    printf("Enter the elements:\n");

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            scanf("%d",&matrix[i][j]);
        }
    }

    printf("Entered matrix:\n");

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            printf("%3d ",matrix[i][j]);
        }
        printf("\n");
    }

    connect(new_socket,(struct sockaddr *)&address,sizeof(address));

    send(new_socket,&N,sizeof(N),0);

    // Send matrix elements one by one
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            send(new_socket,&matrix[i][j],sizeof(int),0);
        }
    }

    int n=recv(new_socket,result,sizeof(result)-1,0);

    if(n>0)
    {
        result[n]='\0';
        printf("Matrix type: %s\n",result);
    }

    close(new_socket);

    return 0;
}

