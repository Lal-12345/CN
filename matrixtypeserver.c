
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 12345

int main()
{
    int server_fd,new_socket;
    struct sockaddr_in address;
    int addrlen=sizeof(address);
    int N,matrix[10][10];

    server_fd=socket(AF_INET,SOCK_STREAM,0);

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    bind(server_fd,(struct sockaddr *)&address,sizeof(address));

    listen(server_fd,3);

    printf("Server waiting...\n");

    new_socket=accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen);

    recv(new_socket,&N,sizeof(N),0);

    // Receive matrix elements one by one
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            recv(new_socket,&matrix[i][j],sizeof(int),0);
        }
    }

    printf("Matrix received from client:\n");

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            printf("%3d ",matrix[i][j]);
        }
        printf("\n");
    }

    int upper=1,lower=1,diagonal=1;

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(i>j && matrix[i][j]!=0)
                upper=0;

            if(i<j && matrix[i][j]!=0)
                lower=0;

            if(i!=j && matrix[i][j]!=0)
                diagonal=0;
        }
    }

    char result[100];

    if(diagonal)
        strcpy(result,"Diagonal Matrix");
    else if(upper)
        strcpy(result,"Upper Triangular");
    else if(lower)
        strcpy(result,"Lower Triangular");
    else
        strcpy(result,"Not a special matrix");

    send(new_socket,result,strlen(result)+1,0);

    printf("Matrix type sent to client\n");

    close(new_socket);
    close(server_fd);

    return 0;
}

