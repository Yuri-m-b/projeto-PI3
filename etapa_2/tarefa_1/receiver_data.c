#include <ioLib.h>
#include <inetLib.h>
#include <semLib.h>
#include <sockLib.h>
#include <stdio.h>
#include <string.h>
#include <taskLib.h>
#include <vxWorks.h>

#define SERVER_PORT 21                // Porta do destino
#define BUFFER_SIZE 1024              // Tamanho do buffer
#define MAX_PACKAGES 10

SEM_ID taskDoneSem; // "Semaforo" para sinal de conclusão da task

// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void receivePackets(void) {
    int sock;
    struct sockaddr_in serverAddr; // Estrutura para guardar o addr do servidor
    char buffer[BUFFER_SIZE];        // Buffer do pacote que será enviado
    int receivedBytes;
    int packetCount = 0;  // Contador de pacotes recebidos
    
    // Criar socket UDP
    sock = socket(AF_INET, SOCK_DGRAM, 0); // Cria um socket para IPV4 e UDP
    if (sock == ERROR) {
        logMsg("Erro ao criar o socket\n");
        return;
    }

    // Configurar endereço do servidor
    memset((char *)&serverAddr, 0, sizeof(serverAddr)); // Inicializa serverAddr struct para zero para evitar valores indesejados
    serverAddr.sin_family = AF_INET;                    // Define o endereço "familia" para o IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);     // Aceita dados do endereço
    serverAddr.sin_port = htons(SERVER_PORT);           // Define o número da porta em bytes


    // Restringe socket para a Port desejada
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == ERROR) {
        logMsg("Socket bind falhou\n");
        close(sock);
        return;
    }

    printf("Esperando por dados...\n");

    // Esperando pacotes...
    while (packetCount < MAX_PACKAGES) {
        receivedBytes = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (receivedBytes == ERROR) {
            logMsg("Erro ao receber pacote\n");
        } 
        else {
            buffer[receivedBytes] = '\0'; // Termine com null se você espera dados de texto
            logMsg("Recebeu dado: %s\n", buffer);
            packetCount++;
        }
    }

    logMsg("Recebido %d pacotes, saindo.\n", packetCount);

    // Fechar socket
    close(sock);
    // Retorna sinal para conclusão da task
    semGive(taskDoneSem);
}

int main() {
    // Cria um "semaforo" binario
    taskDoneSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

    int taskID = taskSpawn("tReceiver", 10, 0, 8000, (FUNCPTR) receivePackets, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (taskID == ERROR) {
        logMsg("Aconteceu um erro ao criar a task.\n");
    } else {
        logMsg("Task criada com sucesso, task ID: %d\n", taskID);

        // Espera pelo sinal do "semaforo"
        semTake(taskDoneSem, WAIT_FOREVER);

        // Termina a task quando o sinal do "semaforo" for recebido
        if (taskClose(taskID) == ERROR) {
            logMsg("Erro terminando a task %d.\n", taskID);
        } else {
            logMsg("Task %d terminada com sucesso.\n", taskID);
        }
    }

    // Clean up do "semaforo""
    semDelete(taskDoneSem);
}
