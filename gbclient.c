#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8090
#define BUFFER_SIZE 1024
#define TIMEOUT 3  // Timeout in seconds

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    struct timeval tv;

    // --- NEW: USER INPUT SECTION ---
    int WINDOW_SIZE, TOTAL_PACKETS;
    printf("Enter the window size: ");
    scanf("%d", &WINDOW_SIZE);
    printf("Enter the total number of packets (frames) to send: ");
    scanf("%d", &TOTAL_PACKETS);
    // -------------------------------

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("\nClient: Connected to server. Starting GBN protocol...\n");

    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    int base = 1;
    int next_to_send = 1;
    int ack, packets_acked = 0;

    while (packets_acked < TOTAL_PACKETS) {
        // Send all packets in the current window
        while (next_to_send < base + WINDOW_SIZE && next_to_send <= TOTAL_PACKETS) {
            memset(buffer, 0, BUFFER_SIZE);
            printf("Client: Sending packet %d\n", next_to_send);
            sprintf(buffer, "%d", next_to_send);
            send(sock, buffer, strlen(buffer) + 1, 0);
            next_to_send++;
        }

        // Wait for ACK
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);

        if (valread > 0) {
            ack = atoi(buffer);
            printf("Client: ACK received for packet %d\n", ack);
            
            // In Go-Back-N, if the received ACK matches the base, slide the window
            if (ack == base) {
                base = ack + 1;
                packets_acked = ack;
            }
        } else {
            // Timeout occurs
            printf("Client: Timeout! Retransmitting from packet %d...\n", base);
            next_to_send = base;  // Go back to the base packet
        }
    }

    printf("\nClient: All %d packets sent and acknowledged successfully.\n", TOTAL_PACKETS);
    close(sock);
    return 0;
}