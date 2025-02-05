#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 20                // Porta usada para a comunicação UDP
#define CHUNK_SIZE 1024        // Tamanho de cada pacote UDP (chunk)
#define HEADER_SIZE 128        // Tamanho do cabeçalho (para timestamp e metadados)
#define OUTPUT_IMAGE "received_image.jpg" // Nome do arquivo de imagem que será salvo
#define OUTPUT_FILE "image_output1.csv"   // Nome do arquivo CSV para salvar as diferenças de tempo

// Função para calcular a diferença de tempo entre o envio e o recebimento do pacote
double calculate_time_difference(struct timespec start, struct timespec end) {
    double diff_sec = (double)(end.tv_sec - start.tv_sec);         // Diferença em segundos
    double diff_nsec = (double)(end.tv_nsec - start.tv_nsec) / 1e9; // Diferença em nanossegundos (convertida para segundos)
    return diff_sec + diff_nsec; // Retorna a diferença total em segundos
}

int main() {
    int sock; // Descritor do socket UDP
    struct sockaddr_in server_addr, client_addr; // Endereço do servidor e do cliente
    socklen_t client_addr_len = sizeof(client_addr); // Tamanho do endereço do cliente
    char packet[CHUNK_SIZE + HEADER_SIZE]; // Pacote que receberá os dados (com cabeçalho e conteúdo)
    char buffer[CHUNK_SIZE]; // Buffer para armazenar os dados recebidos
    struct timespec receive_time, send_time; // Estruturas para armazenar os tempos de recebimento e envio
    FILE *output_file, *image_file; // Ponteiros para os arquivos de saída (CSV e imagem)

    int total_packets = -1; // Número total de pacotes a serem recebidos
    int received_packets = 0; // Número de pacotes recebidos até o momento
    int expected_packet_id = 0; // ID esperado do próximo pacote
    int *packet_tracker = NULL; // Array para rastrear os pacotes recebidos

    // Abre os arquivos de saída (CSV e imagem)
    output_file = fopen(OUTPUT_FILE, "w");
    image_file = fopen(OUTPUT_IMAGE, "wb");
    if (!output_file || !image_file) { // Verifica se os arquivos foram abertos com sucesso
        perror("Falha ao abrir os arquivos");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Index;Time\n"); // Escreve o cabeçalho no arquivo CSV

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Falha na criação do socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr)); // Zera a estrutura do servidor
    server_addr.sin_family = AF_INET; // Define o tipo de endereço como IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Permite qualquer endereço IP
    server_addr.sin_port = htons(PORT); // Define a porta do servidor

    // Realiza o bind do socket
    if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for image packets...\n");

    while (1) {
        // Recebe um pacote
        int len = recvfrom(sock, packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (len <= HEADER_SIZE) break; // Para de receber pacotes se não houver dados

        // Obtém o horário atual do recebimento
        clock_gettime(CLOCK_REALTIME, &receive_time);

        // Extrai os metadados do pacote (timestamp de envio, ID do pacote e número total de pacotes)
        int packet_id, total_count;
        sscanf(packet, "%ld.%ld;%d;%d", &send_time.tv_sec, &send_time.tv_nsec, &packet_id, &total_count);

        // Aloca o tracker de pacotes se ainda não foi feito
        if (total_packets == -1) {
            total_packets = total_count;
            packet_tracker = calloc(total_packets, sizeof(int));
            if (!packet_tracker) {
                perror("Failed to allocate memory for packet tracker");
                exit(EXIT_FAILURE);
            }
        }

        // Copia os dados do pacote para o buffer
        memcpy(buffer, packet + HEADER_SIZE, len - HEADER_SIZE);

        // Valida e rastreia os pacotes recebidos
        if (packet_id >= 0 && packet_id < total_packets && !packet_tracker[packet_id]) {
            packet_tracker[packet_id] = 1; // Marca o pacote como recebido
            fwrite(buffer, 1, len - HEADER_SIZE, image_file); // Escreve os dados no arquivo de imagem
            double time_diff = calculate_time_difference(send_time, receive_time); // Calcula a diferença de tempo
            fprintf(output_file, "%d;%.9f\n", packet_id, time_diff);
            fflush(output_file); // Força a escrita no arquivo
            received_packets++; // Incrementa o contador de pacotes recebidos
            printf("Packet %d received. Time difference: %.9f seconds\n", packet_id, time_diff);
        }

        // Verifica se todos os pacotes foram recebidos
        if (received_packets == total_packets) break;
    }

    // Verifica pacotes ausentes
    if (packet_tracker) {
        printf("Checking for missed packets...\n");
        for (int i = 0; i < total_packets; i++) {
            if (!packet_tracker[i]) {  // Se o pacote não foi recebido
                printf("Missed packet: %d\n", i);
            }
        }
        free(packet_tracker); // Libera a memória alocada para o tracker
    }

    fclose(output_file); // Fecha o arquivo CSV
    fclose(image_file); // Fecha o arquivo de imagem
    close(sock); // Fecha o socket

    printf("Image received successfully.\n");
    return 0;
}

