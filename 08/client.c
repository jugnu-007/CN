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
#define MSG_SIZE 512

int main() {
    int sd;
    ssize_t bytes_sent, bytes_received;
    char msg[MSG_SIZE], buffer[BUFFER_SIZE];
    struct sockaddr_in server;

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    
    while (1) {
        printf("\nClient >> Enter your message: ");
        scanf("%s", msg);

        if (!strcmp(msg, "exit")) {
            break;
        }

        sendto(sd, msg, strlen(msg) + 1, 0, (struct sockaddr*) &server, sizeof(server));

        memset(buffer, 0x0, BUFFER_SIZE);
        recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server, &(socklen_t){sizeof(server)});

        printf("Client >> Server response: %s\n", buffer);
    }

    close(sd);
    return 0;
}
