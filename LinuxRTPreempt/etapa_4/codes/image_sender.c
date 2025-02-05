#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 20               // Número da porta para enviar os dados
#define TARGET_IP "192.168.0.35" // Substituir pelo endereço IP do receiver
#define CHUNK_SIZE 1024          // Tamanho de cada pacote UDP (chunk)
#define HEADER_SIZE 128          // Espaço reservado para timestamp e metadados

// Função para enviar uma imagem em pacotes UDP
void send_image(const char *file_path, int sock, struct sockaddr_in *server_addr) {
    FILE *file = fopen(file_path, "rb"); // Abre o arquivo da imagem no modo leitura binária
    if (!file) { // Verifica se o arquivo foi aberto com sucesso
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char buffer[CHUNK_SIZE]; // Buffer para armazenar os dados lidos do arquivo
    char packet[CHUNK_SIZE + HEADER_SIZE]; // Buffer para armazenar o pacote completo (metadados + dados)
    size_t bytes_read; // Quantidade de bytes lidos do arquivo
    struct timespec send_time; // Estrutura para armazenar o horário de envio
    socklen_t addr_len = sizeof(*server_addr); // Tamanho do endereço do server
    int packet_id = 0; // Identificador do pacote atual
    int total_packets = 0; // Total de pacotes a serem enviados

    // Calcula o número total de pacotes com base no tamanho do arquivo e do chunk
    fseek(file, 0, SEEK_END); // Move o ponteiro para o final do arquivo
    long file_size = ftell(file); // Obtém o tamanho do arquivo
    rewind(file); // Retorna o ponteiro para o início do arquivo
    total_packets = (file_size + CHUNK_SIZE - 1) / CHUNK_SIZE; // Calcula o total de pacotes necessários

    printf("Starting to send image: %s\n", file_path);
    printf("Total packets: %d\n", total_packets);

    // Loop para enviar os dados em chunks
    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, file)) > 0) { // Lê um chunk do arquivo
        // Obtém o horário atual
        clock_gettime(CLOCK_REALTIME, &send_time);

        // Prepara o pacote com timestamp, metadados e dados
        snprintf(packet, HEADER_SIZE, "%ld.%ld;%d;%d;", 
                 send_time.tv_sec, send_time.tv_nsec, packet_id, total_packets);
        memcpy(packet + HEADER_SIZE, buffer, bytes_read); // Copia os dados para o pacote

        // Envia o pacote
        if (sendto(sock, packet, HEADER_SIZE + bytes_read, 0, (struct sockaddr *)server_addr, addr_len) < 0) {
            perror("Failed to send packet");
        } else {
            printf("Sent packet %d/%d\n", packet_id + 1, total_packets); // Log do progresso
        }

        packet_id++;
        usleep(1000); // Small delay to simulate network conditions
    }

    fclose(file);
    printf("Image sent successfully.\n");
}

int main() {
    int sock; // Descritor do socket
    struct sockaddr_in server_addr; // Estrutura para armazenar o endereço do servidor

    // Cria um socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configura o endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr)); // Zera a estrutura
    server_addr.sin_family = AF_INET; // Define o tipo de endereço como IPv4
    server_addr.sin_port = htons(PORT); // Define a porta em formato de rede
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) { // Converte o IP para formato binário
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Chama a função para enviar a imagem
    send_image("bmw_1080.jpg", sock, &server_addr);

    close(sock); // Fecha o socket
    return 0;
}

