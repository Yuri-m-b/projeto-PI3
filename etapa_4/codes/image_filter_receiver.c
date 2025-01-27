#include <stdio.h>       // Biblioteca padrão para entrada e saída
#include <stdlib.h>      // Biblioteca padrão para funções utilitárias, como malloc e exit
#include <string.h>      // Biblioteca para manipulação de strings
#include <unistd.h>      // Biblioteca para funções POSIX, como close e usleep
#include <arpa/inet.h>   // Biblioteca para manipulação de endereços de rede
#include <time.h>        // Biblioteca para manipulação de datas e horários
#include <sys/select.h>  // Biblioteca para monitorar múltiplos descritores de arquivo

#define PORT 20                 // Porta de escuta do servidor UDP
#define CHUNK_SIZE 1024         // Tamanho do bloco de dados a ser processado
#define HEADER_SIZE 128         // Tamanho do cabeçalho dos pacotes
#define OUTPUT_IMAGE "received2_image.jpg" // Nome do arquivo de imagem de saída
#define OUTPUT_FILE "image_output3.csv"    // Nome do arquivo CSV para saída de dados
#define TIMEOUT_SECONDS 6        // Tempo limite para receber pacotes (em segundos)

// Função para calcular a diferença de tempo em segundos
double calculate_time_difference(struct timespec start, struct timespec end) {
    double diff_sec = (double)(end.tv_sec - start.tv_sec);         // Diferença em segundos inteiros
    double diff_nsec = (double)(end.tv_nsec - start.tv_nsec) / 1e9; // Diferença em nanosegundos convertida para segundos
    return diff_sec + diff_nsec;                                   // Soma das diferenças em segundos e nanosegundos
}

// Função para aplicar filtro de escala de cinza nos dados RGB
void apply_grayscale_filter(unsigned char *chunk, int length) {
    for (int i = 0; i < length; i += 3) { // Itera pelos dados em tripletos (R, G, B)
        if (i + 2 < length) {             // Garante que um triplo completo está disponível
            unsigned char r = chunk[i];               // Componente vermelho
            unsigned char g = chunk[i + 1];           // Componente verde
            unsigned char b = chunk[i + 2];           // Componente azul
            unsigned char gray = (r + g + b) / 3;     // Calcula o valor em escala de cinza
            chunk[i] = chunk[i + 1] = chunk[i + 2] = gray; // Substitui os valores RGB pelo cinza
        }
    }
}

int main() {
    int sock;                                // Descritor de socket
    struct sockaddr_in server_addr, client_addr; // Estruturas para endereços do servidor e cliente
    socklen_t client_addr_len = sizeof(client_addr); // Tamanho do endereço do cliente
    char packet[CHUNK_SIZE + HEADER_SIZE];   // Buffer para armazenar pacotes recebidos
    char buffer[CHUNK_SIZE];                 // Buffer para armazenar dados processados
    struct timespec receive_time, send_time; // Estruturas para armazenar tempos de envio e recepção
    FILE *output_file, *image_file;          // Ponteiros para arquivos de saída

    int total_packets = -1;                  // Número total de pacotes (inicialmente desconhecido)
    int received_packets = 0;                // Contador de pacotes recebidos
    int *packet_tracker = NULL;              // Array para rastrear pacotes recebidos
    int count = 0;                           // Contador de pacotes perdidos

    // Abre os arquivos de saída para escrita
    output_file = fopen(OUTPUT_FILE, "w");
    image_file = fopen(OUTPUT_IMAGE, "wb");
    if (!output_file || !image_file) {       // Verifica se os arquivos foram abertos com sucesso
        perror("Failed to open files");      // Exibe mensagem de erro, se necessário
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Packet ID;Time Difference (seconds)\n"); // Cabeçalho do arquivo CSV

    // Cria o socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // Verifica se a criação foi bem-sucedida
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Inicializa a estrutura de endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;             // Família de endereços (IPv4)
    server_addr.sin_addr.s_addr = INADDR_ANY;     // Aceita conexões de qualquer IP
    server_addr.sin_port = htons(PORT);           // Porta de escuta

    // Associa o socket ao endereço do servidor
    if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");                    // Exibe mensagem de erro, se necessário
        close(sock);                              // Fecha o socket
        exit(EXIT_FAILURE);
    }

    printf("Waiting for image packets...\n");     // Indica que o servidor está aguardando pacotes

    while (1) {
        // Configura o conjunto de descritores de arquivo para monitoramento
        fd_set read_fds;
        FD_ZERO(&read_fds);               // Inicializa o conjunto de descritores
        FD_SET(sock, &read_fds);          // Adiciona o socket ao conjunto

        // Define o tempo limite para esperar por dados
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SECONDS; // Segundos
        timeout.tv_usec = 0;              // Microsegundos

        // Aguarda por atividade no socket com timeout
        int activity = select(sock + 1, &read_fds, NULL, NULL, &timeout);

        if (activity == 0) { // Timeout ocorreu
            printf("No packets received for %d seconds. Stopping...\n", TIMEOUT_SECONDS);
            break;
        } else if (activity < 0) { // Erro no select
            perror("Select error");
            break;
        }

        // Recebe um pacote do socket
        int len = recvfrom(sock, packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (len <= HEADER_SIZE) break; // Se o pacote for menor que o cabeçalho, termina

        clock_gettime(CLOCK_REALTIME, &receive_time); // Registra o horário de recepção

        // Lê os dados do cabeçalho
        int packet_id, total_count;
        sscanf(packet, "%ld.%ld;%d;%d", &send_time.tv_sec, &send_time.tv_nsec, &packet_id, &total_count);

        if (total_packets == -1) { // Inicializa ao receber o primeiro pacote
            total_packets = total_count;
            packet_tracker = calloc(total_packets, sizeof(int)); // Aloca memória para rastrear pacotes
            if (!packet_tracker) {
                perror("Failed to allocate memory for packet tracker");
                exit(EXIT_FAILURE);
            }
        }

        memcpy(buffer, packet + HEADER_SIZE, len - HEADER_SIZE); // Copia os dados do pacote para o buffer

        // Processa o pacote se ainda não foi recebido
        if (packet_id >= 0 && packet_id < total_packets && !packet_tracker[packet_id]) {
            packet_tracker[packet_id] = 1; // Marca o pacote como recebido

            // Aplica o filtro de escala de cinza
            apply_grayscale_filter((unsigned char *)buffer, len - HEADER_SIZE);

            fwrite(buffer, 1, len - HEADER_SIZE, image_file); // Escreve os dados no arquivo de imagem
            double time_diff = calculate_time_difference(send_time, receive_time); // Calcula o tempo de transferência
            fprintf(output_file, "%d;%.9f\n", packet_id, time_diff); // Salva no arquivo CSV
            fflush(output_file);

            received_packets++; // Incrementa o contador de pacotes recebidos
            printf("Packet %d received. Time difference: %.9f seconds\n", packet_id, time_diff);
        }

        if (received_packets == total_packets) break; // Finaliza se todos os pacotes foram recebidos

        // usleep(50000); // 50ms delay 
    }

    // Verifica pacotes perdidos
    if (packet_tracker) {
        printf("Checking for missed packets...\n");
        for (int i = 0; i < total_packets; i++) {
            if (!packet_tracker[i]) { // Imprime os pacotes que não foram recebidos
                printf("Missed packet: %d\n", i);
                count++;
            }
        }
        free(packet_tracker); // Libera a memória do rastreador
    }

    fclose(output_file); // Fecha o arquivo CSV
    fclose(image_file);  // Fecha o arquivo de imagem
    close(sock);         // Fecha o socket

    printf("Total Packets lost: %d\n", count); // Exibe o total de pacotes perdidos
    printf("Transference finished.\n");       // Indica o fim da transferência
    return 0;
}

