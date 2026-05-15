#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8090
#define BUFFER_SIZE 1024
#define LOSS_PROBABILITY 30

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    int expected = 1;   // next expected packet
    int last_ack = 0;   // last acknowledged packet

    srand(time(0));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for connection...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Connection established.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);

        if (valread <= 0) {
            printf("Server: Client disconnected.\n");
            break;
        }

        int packet = atoi(buffer);
        printf("Server: Received packet %d\n", packet);

        // Simulate ACK loss
        if (rand() % 100 < LOSS_PROBABILITY) {
            printf("Server: ACK lost!\n\n");
            continue;
        }

        // STRICT GBN LOGIC
        if (packet == expected) {
            printf("Server: Accepted packet %d\n", packet);
            last_ack = packet;
            expected++;
        } else {
            printf("Server: Discarded packet %d (Expected %d)\n", packet, expected);
        }

        sleep(1);

        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "%d", last_ack);   // cumulative ACK
        send(new_socket, buffer, strlen(buffer) + 1, 0);

        printf("Server: ACK sent for packet %d\n\n", last_ack);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}