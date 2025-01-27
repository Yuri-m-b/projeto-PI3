#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 20             // Porta onde o receptor escutará mensagens
#define BUFFER_SIZE 256        // Tamanho máximo do buffer para mensagens recebidas
#define OUTPUT_FILE "string_output1.csv" // Nome do arquivo CSV onde os resultados serão gravados


// Função para calcular a diferença de tempo em segundos
double calculate_time_difference(struct timespec start, struct timespec end) {
    double diff_sec = (double)(end.tv_sec - start.tv_sec);          // Diferença em segundos
    double diff_nsec = (double)(end.tv_nsec - start.tv_nsec) / 1e9; // Diferença em nanosegundos convertida para segundos
    return diff_sec + diff_nsec;                                   // Soma as diferenças para obter o total
}

int main() {
    int sock;                             // Socket para comunicação UDP
    char buffer[BUFFER_SIZE];             // Buffer para armazenar mensagens recebidas
    struct sockaddr_in server_addr, client_addr; // Estruturas para endereços do servidor e cliente
    socklen_t client_addr_len = sizeof(client_addr); // Tamanho do endereço do cliente
    FILE *output_file;                    // Ponteiro para o arquivo de saída

    // Abre o arquivo CSV para escrita
    output_file = fopen(OUTPUT_FILE, "w");
    if (!output_file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    fprintf(output_file, "Index;Time\n");

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr)); // Inicializa a estrutura com zeros
    server_addr.sin_family = AF_INET;            // Define a família de endereços (IPv4)
    server_addr.sin_addr.s_addr = INADDR_ANY;    // Aceita conexões de qualquer endereço
    server_addr.sin_port = htons(PORT);          // Define a porta para escuta

    // Faz o bind do socket para o endereço e porta configurados
    if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for packets...\n");

    for (int i = 1; i <= 100; i++) {
        struct timespec receive_time;  // Armazena o horário de recebimento
        struct timespec send_time;     // Armazena o horário de envio (extraído da mensagem)
        
        // Recebe uma mensagem do cliente
        int len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (len < 0) { // Verifica se houve falha na recepção
            perror("Receive failed");
            continue;
        }
        buffer[len] = '\0'; // Garante que o buffer seja uma string válida

        // Extrai o horário de envio da mensagem recebida
        if (sscanf(buffer, "Hello everyone, this is my static message. Time: %ld.%ld", 
                   &send_time.tv_sec, &send_time.tv_nsec) != 2) {
            fprintf(stderr, "Failed to parse time from message\n");
            continue;
        }

        // Obtém o horário de recebimento
        clock_gettime(CLOCK_REALTIME, &receive_time);

        // Calcula a diferença de tempo entre o envio e o recebimento
        double time_diff = calculate_time_difference(send_time, receive_time);

        // Imprime e registra os resultados
        printf("Message received: \"%s\"\n", buffer);
        printf("The difference: %.9f seconds\n", time_diff);
        fprintf(output_file, "%d;%.9f\n", i, time_diff);  // Salva no arquivo CSV
    }

    fclose(output_file);
    close(sock);
    return 0;
}

