#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8090
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char packet[50];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Client: Connected to server\n");

    while (1) {

        printf("Enter packet (or type exit): ");
        fgets(packet, sizeof(packet), stdin);
        packet[strcspn(packet, "\n")] = 0;

        while (1) {   // 🔁 Retransmission loop

            send(sock, packet, strlen(packet), 0);

            if (strcmp(packet, "exit") == 0) {
                printf("Client: Exit sent\n");
                close(sock);
                return 0;
            }

            fd_set readfds;
            struct timeval tv;

            FD_ZERO(&readfds);
            FD_SET(sock, &readfds);

            tv.tv_sec = 3;
            tv.tv_usec = 0;

            int retval = select(sock + 1, &readfds, NULL, NULL, &tv);

            if (retval > 0) {
                memset(buffer, 0, BUFFER_SIZE);
                read(sock, buffer, BUFFER_SIZE);

                if (strcmp(buffer, "ACK") == 0) {
                    printf("Client: ACK received\n\n");
                    break;  // ✅ go to next packet
                }
            } else {
                printf("Client: Timeout! Retransmitting...\n\n");
            }
        }
    }

    return 0;
}
