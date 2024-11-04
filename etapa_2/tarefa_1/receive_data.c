#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>
#include <string.h>
#include <sockLib.h>
#include <inetLib.h>
#include <netinet/in.h>

#define PORT 8080           // Port number to listen on
#define BUFFER_SIZE 1024    // Buffer size for incoming data

// Function to read a packet from the network
void readNetworkPacket(void) {
    int sock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(clientAddr);
    
    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return;
    }

    // Prepare the sockaddr_in structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;         // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept any incoming address
    serverAddr.sin_port = htons(PORT);       // Port number

    // Bind the socket
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        close(sock);
        return;
    }

    printf("Listening for packets on port %d...\n", PORT);

    // Wait to receive a single packet
    ssize_t bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE, 0,
                                     (struct sockaddr *)&clientAddr, &addrLen);
    if (bytesReceived < 0) {
        perror("recvfrom failed");
    } else {
        // Process and print the received packet
        buffer[bytesReceived] = '\0';  // Null-terminate the received data
        printf("Received packet from %s:%d: %s\n",
               inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port),
               buffer);
    }

    // Clean up and close the socket after receiving one message
    close(sock);
    printf("Socket closed after receiving a message.\n");
}

// Entry point to create the task
void startNetworkTask(void) {
    taskSpawn("tReadNetworkPacket", 100, 0, 10000, (FUNCPTR)readNetworkPacket, 
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

int main() {
    startNetworkTask();  // Start the task to read network packets
    return 0;
}

