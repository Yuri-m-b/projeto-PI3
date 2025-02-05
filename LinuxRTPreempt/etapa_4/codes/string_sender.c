#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sched.h>

#define PORT 20 // Porta utilizada para comunicação UDP
#define TARGET_IP "192.168.0.35" // Endereço IP do receptor
#define MESSAGE_SIZE 128 // Tamanho máximo da mensagem enviada
#define NUM_MESSAGES 100 // Quantidade de mensagens enviada

// Função para configurar a prioridade em tempo real
// Usar a prioridade em tempo real garante que:

// É menos provável que o programa seja interrompido por outros processos.
// A precisão do tempo de envio de pacotes (a cada 1 segundo) é mantida de forma mais confiável.

// SCHED_FIFO é um agendador em tempo real do tipo primeiro a entrar, primeiro a sair:
// Ele executa primeiro as tarefas com a prioridade mais alta.
// Tarefas com a mesma prioridade são executadas na ordem em que são agendadas.
// Uma tarefa em SCHED_FIFO será executada até que ceda voluntariamente a CPU (por exemplo, dormindo).
void set_real_time_priority() {
    struct sched_param param;
    param.sched_priority = 50; // Definir prioridade (intervalo: 1-99, sendo 99 mais alta)
    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) { // Configura o agendador para FIFO
        perror("Failed to set real-time priority");
        exit(EXIT_FAILURE); // Encerra o programa em caso de falha
    }
}

// Função para enviar pacotes com o horário atual
void *send_time_packet(void *arg) {
    int sock;
    struct sockaddr_in server_addr;
    char message[MESSAGE_SIZE];
    time_t raw_time;
    struct tm *time_info;
    struct timespec send_time;

    // Criação do socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Família de endereços IPv4
    server_addr.sin_port = htons(PORT); // Conversão da porta para formato de rede
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) { // Conversão do endereço IP
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Recebe tempo atual
        clock_gettime(CLOCK_REALTIME, &send_time);

        // Prepara a mensagem: string + timestamp
        snprintf(message, MESSAGE_SIZE, "Hello everyone, this is my static message. Time: %ld.%09ld", 
                 send_time.tv_sec, send_time.tv_nsec);

        // Envia mensagem
        if (sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)  {
            perror("Failed to send message");
        } else {
            printf("Sending string: \"%s\"\n", message);
        }
        usleep(50000); // 50ms delay
    }

    close(sock); // Fecha o socket
    return NULL;
}

int main() {
    pthread_t sender_thread;

    set_real_time_priority(); // Configura a prioridade em tempo real

    // Cria uma nova thread para enviar pacotes
    if (pthread_create(&sender_thread, NULL, send_time_packet, NULL) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    // Aguarda a thread terminar (o que nunca ocorre nesse caso)
    pthread_join(sender_thread, NULL);
    return 0;
}

