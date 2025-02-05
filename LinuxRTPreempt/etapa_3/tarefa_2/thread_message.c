#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

#define PORT 20                  // Porta onde os pacotes serão enviados
#define TARGET_IP "192.168.0.35" // Substitua pelo IP do receptor
#define MESSAGE_SIZE 128         // Tamanho máximo da mensagem
#define THREAD_PRIORITY_1 60     // Prioridade do primeiro thread
#define THREAD_PRIORITY_2 50     // Prioridade do segundo thread

// Function to set real-time priority for a thread
void set_real_time_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority; // Define a prioridade
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        perror("Failed to set real-time priority");
        exit(EXIT_FAILURE);
    }
}

// Função executada pela thread 1
// Função para enviar pacotes com timestamp
void *send_time_packet(void *arg) {
    int sock;
    int i=0;
    struct sockaddr_in server_addr;
    char message[MESSAGE_SIZE];
    time_t raw_time;
    struct tm *time_info;

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    set_real_time_priority(THREAD_PRIORITY_1); // Define prioridade

    while (i<10) {
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(message, MESSAGE_SIZE, "Thread 1: %Y-%m-%d %H:%M:%S", time_info);

        if (sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        } else {
            printf("Thread 1 Sent: %s\n", message);
        }
        sleep(1); // Envia a cada segundo
        i++;
    }

    close(sock);
    return NULL;
}

// Função executada pela thread 2
// Função que envia uma mensagem estatica
void *send_static_message(void *arg) {
    int sock;
    int i = 0;
    struct sockaddr_in server_addr;
    char *message = "Thread 2: Static Message";

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    set_real_time_priority(THREAD_PRIORITY_2); // Define prioridade

    while (i<10) {
        if (sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send message");
        } else {
            printf("Thread 2 Sent: %s\n", message);
        }
        sleep(2); // // Envia a cada 2 segundos
        i++;
    }

    close(sock);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Cria os threads com diferentes prioridades e tarefas
    if (pthread_create(&thread1, NULL, send_time_packet, NULL) != 0) {
        perror("Failed to create thread 1");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, NULL, send_static_message, NULL) != 0) {
        perror("Failed to create thread 2");
        return EXIT_FAILURE;
    }

    // Espera threads terminar
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}


