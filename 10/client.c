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
#define SIZE 512

int main() {
    int flag = 0;
    int sd;
    char msg[SIZE], buffer[SIZE], ack[SIZE];
    struct sockaddr_in server;
    int win_size, i, j, count;

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sd, (struct sockaddr*)&server, sizeof(server));
    
    while (1) {

        printf("\nClient >> Enter your message: ");
        scanf("%s", msg);

        printf("Client >> Enter window size: ");
        scanf("%d", &win_size);


        memset(buffer, 0x0, SIZE);
        j = 0;
        count = 0;
        for (i=0; i<strlen(msg); ++i) {
            if (j<win_size) {
                buffer[j++] = msg[i];
            }
            if (j==win_size || i==strlen(msg)-1) {
                buffer[j] = '\0';
                send(sd, buffer, strlen(buffer) + 1, 0);
                if (!strcmp(buffer, "exit")) {
                    flag = 1;
                    break;
                }

                memset(ack, 0x0, SIZE);
                recv(sd, ack, SIZE, 0);
                printf("Client >> %s %d\n", ack, count++);
                j = 0;
            }
        }

        if (flag) {
            break;
        }
    }

    printf("Client >> Manual terminating from client side\n");
    close(sd);
    return 0;
}