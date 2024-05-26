#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    char buffer[BUFFER_SIZE];
    int listening_sd;
    struct sockaddr_in client, server;
    ssize_t bytes_received, bytes_sent;
    char * response;

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    listening_sd = socket(AF_INET, SOCK_DGRAM, 0);
    
    bind(listening_sd, (struct sockaddr *)&server, sizeof(server));

    printf("Server is listening on port %u\n", SERVER_PORT);

    while (1) {
        memset(buffer, 0x0, BUFFER_SIZE);
        recvfrom(listening_sd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client, &(socklen_t){sizeof(client)});

        printf("\nServer >> %s:%u sent request\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        if (!strcmp(buffer, "time")) {
            response = ctime(&(time_t){time(NULL)});
        } else {
            response = "Bad Request";
        }

        sendto(listening_sd, response, strlen(response) + 1, 0, (struct sockaddr*)&client, sizeof(client));
    }

    close(listening_sd);

    return 0;
}