#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    char buffer[BUFFER_SIZE];
    int listening_sd, connection_sd;
    struct sockaddr_in client, server;
    const char * ack = "ACKNOWLEDGEMENT RECEIVED";

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    listening_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        return 1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(listening_sd, 5) < 0) {
        perror("listen");
        return 1;
    }

    printf("Server is listening on port %u\n", SERVER_PORT);

    while (1) {
        connection_sd = accept(listening_sd, (struct sockaddr*) &client, &(socklen_t){sizeof(client)});

        if (connection_sd < 0) {
            perror("accept");
            continue;
        }
        printf("\nServer >> %s:%u is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        while (1) {
            memset(buffer, 0x0, BUFFER_SIZE);
            if (recv(connection_sd, buffer, BUFFER_SIZE - 1, 0) < 0) {
                perror("recv");
                continue;
            }

            printf("Server >> Data Received: ");
            puts(buffer);

            if (!strcmp(buffer, "exit")) {
                break;
            }
            
            if (send(connection_sd, ack, strlen(ack) + 1, 0) < 0) {
                perror("send");
                continue;
            }
        }

        printf("\nServer >> %s:%u is manually disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        close(connection_sd);
    }

    close(listening_sd);

    return 0;
}
