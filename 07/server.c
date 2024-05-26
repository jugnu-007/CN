#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    int sd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client, server;
    int clen = sizeof(client);

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sd, (struct sockaddr*)&server, sizeof(server));

    printf("The server is running on port %d\n", SERVER_PORT);
    
    while (1) {
        memset(buffer, 0x0, BUFFER_SIZE);
        recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &client, &clen);
        sendto(sd, buffer, strlen(buffer)+1, 0, (struct sockaddr*) &client, sizeof(client));
    }

    close(sd);
    return 0;
}
