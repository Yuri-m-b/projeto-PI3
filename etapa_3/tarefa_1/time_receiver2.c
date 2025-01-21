#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 20 // Porta onde o servidor vai escutar por pacotes UDP
#define BUFFER_SIZE 128 // Tamanho máximo do buffer para receber mensagens

int main() {
    int sock;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr; // Estruturas para armazenar endereços do servidor e cliente
    socklen_t client_addr_len = sizeof(client_addr); // Tamanho do endereço do cliente

    // Cria um socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // AF_INET para IPv4, SOCK_DGRAM para UDP
        perror("Socket creation failed"); // Exibe uma mensagem de erro caso a criação do socket falhe
        exit(EXIT_FAILURE); // Sai do programa em caso de erro
    }

    // Zera a estrutura de endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Define a família de endereços como IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões de qualquer endereço IP
    server_addr.sin_port = htons(PORT); // Converte o número da porta para o formato de rede

    // Associa o socket ao endereço e porta especificados
    if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed"); // Exibe uma mensagem de erro caso o bind falhe
        close(sock); // Fecha o socket
        exit(EXIT_FAILURE); // Sai do programa em caso de erro
    }

    printf("Waiting for packets...\n");

    // Loop infinito para receber pacotes
    while (1) {
        // Recebe uma mensagem do cliente
        int len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (len < 0) {
            perror("Receive failed");
        } else {
            buffer[len] = '\0'; // Certifica-se de que a mensagem recebida seja uma string válida
            printf("Received: %s\n", buffer);

            // Obtém o horário atual no receptor
            time_t receiver_time = time(NULL);
            struct tm *receiver_time_info = localtime(&receiver_time);

            // Formata o horário atual no receptor para comparação
            char current_time_str[BUFFER_SIZE];
            strftime(current_time_str, BUFFER_SIZE, "Current Time: %Y-%m-%d %H:%M:%S", receiver_time_info);

            // Compara o horário recebido com o horário local
            if (strcmp(buffer, current_time_str) == 0) {
                printf("The times coincide.\n");
            } else {
                printf("Difference detected between times.\n");
                printf("Receiver hours: %s\n", current_time_str);
            }
            sleep(2);
        }
    }

    close(sock);
    return 0;
}

