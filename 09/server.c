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
    pid_t pid;
    int listening_sd, connection_sd;
    struct sockaddr_in client, server;
    ssize_t bytes_received, bytes_sent;

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    listening_sd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listening_sd, (struct sockaddr *)&server, sizeof(server));
    listen(listening_sd, 5);

    printf("Server is listening on port %u\n", SERVER_PORT);

    while (1) {
        connection_sd = accept(listening_sd, (struct sockaddr*) &client, &(socklen_t){sizeof(client)});
        printf("\nServer >> %s:%u connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Mutiprocessing Implementation
        pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            return 1;
        } else if (pid == 0) {
            do {
                memset(buffer, 0x0, BUFFER_SIZE);
                recv(connection_sd, buffer, BUFFER_SIZE - 1, 0);
                send(connection_sd, buffer, strlen(buffer) + 1, 0);
            } while (strcmp(buffer, "exit"));

            printf("\nServer >> %s:%u disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }
        
        close(connection_sd);
    }

    close(listening_sd);

    return 0;
}
