#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sched.h>

#define PORT 20                  // Porta onde os pacotes serão enviados
#define TARGET_IP "192.168.0.35" // Substitua pelo IP do receptor
#define MESSAGE_SIZE 128         // Tamanho máximo da mensagem
#define THREAD_PRIORITY_1 60     // Prioridade do primeiro thread
#define THREAD_PRIORITY_2 50     // Prioridade do segundo thread

// Função para configurar a prioridade em tempo real de um thread
void set_real_time_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority; // Define a prioridade
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0) {
        perror("Failed to set real-time priority");
        exit(EXIT_FAILURE);
    }
}

// Função para enviar pacotes com timestamp
void *send_time_packet(void *arg) {
    int thread_id = *(int *)arg; // Identifica qual thread está enviando
    int sock;
    int i = 0;
    struct sockaddr_in server_addr;
    char message[MESSAGE_SIZE];
    time_t raw_time;
    struct tm *time_info;

    // Defina um offset diferente para cada thread
    int time_offset = thread_id * 5; // Offset em segundos (exemplo: Thread 1 = 5s, Thread 2 = 10s)

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid or unsupported address");
        exit(EXIT_FAILURE);
    }

    while (i < 10) {
        time(&raw_time);
        raw_time += time_offset; // Adiciona o offset ao horário atual
        time_info = localtime(&raw_time);
        snprintf(message, MESSAGE_SIZE, "Thread %d: %s", thread_id, asctime(time_info));

        if (sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Receive failed");
        } else {
            printf("Thread %d Send: %s", thread_id, message);
        }

        sleep(1); // Envia a cada 1 segundo
        i++;
    }

    close(sock);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread1_id = 1;
    int thread2_id = 2;

    // Cria os threads com diferentes prioridades (Identificadores thread1_id e thread2_id)
    // Ambos thread executam a mesma função
    // Cada thread utiliza seu id único para aplicar um offset no seu tempo
    // --thread1_id = 1 ou seja time_offset = 1 * 5 = 5 segundos 
    // --thread2_id = 2 ou seja time_offset = 2 * 5 = 10 segundos
    // Ambos threads estão sendo executados de forma paralela
    if (pthread_create(&thread1, NULL, send_time_packet, &thread1_id) != 0) {
        perror("Fail to create thread 1");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, NULL, send_time_packet, &thread2_id) != 0) {
        perror("Fail to create thread 2");
        return EXIT_FAILURE;
    }

    // Configura prioridade em tempo real para cada thread
    set_real_time_priority(THREAD_PRIORITY_1); // Configura prioridade do thread 1
    pthread_join(thread1, NULL);

    set_real_time_priority(THREAD_PRIORITY_2); // Configura prioridade do thread 2
    pthread_join(thread2, NULL);

    return 0;
}

