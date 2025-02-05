#include <stdio.h>
#include <string.h>
#include <semLib.h>
#include <vxWorks.h>
#include <taskLib.h>
#include <sockLib.h>
#include <inetLib.h>
#include <hostLib.h>
#include <ioLib.h>

#define SERVER_IP "192.168.0.113"  // IP do destino
#define SERVER_PORT 21             // Porta do destino
#define BUF_SIZE 1024              // Tamanho do buffer

SEM_ID taskDoneSem; // "Semaforo" para sinal de conclusão da task

void sendPacket(void)
{
    int sock;
    struct sockaddr_in serverAddr; // Estrutura para guardar o addr do servidor
    char sendBuf[BUF_SIZE];        // Buffer do pacote que será enviado
    int bytesSent;

    // Criar socket UDP
    sock = socket(AF_INET, SOCK_DGRAM, 0);  // Cria um socket para IPV4 e UDP
    if (sock < 0)
    {
        printf("Erro ao criar o socket\n");
        return;
    }

    // Configurar endereço do servidor
    bzero((char *)&serverAddr, sizeof(serverAddr));    // Define a estrutura do server para zero
    serverAddr.sin_family = AF_INET;                   // Define o endereço "familia" para o IPv4
    serverAddr.sin_port = htons(SERVER_PORT);          // Define a PORT (convertendo para network bit)
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // Converte IP de string para binario

    // Cria a mensagem a ser enviada
    strcpy(sendBuf, "Olá, esse é um pacote de teste enviado pela rede!");

    // Envia o pacote
    bytesSent = sendto(sock, sendBuf, strlen(sendBuf), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bytesSent < 0)
    {
        printf("Erro ao enviar o pacote\n");
        close(sock);
        return;
    }

    printf("Pacote enviado com sucesso, %d bytes enviados.\n", bytesSent);

    // Fechar socket
    close(sock);
    // Retorna sinal para conclusão da task
    semGive(taskDoneSem);
}

int main()
{
    // Cria uma "semaforo" binario
    taskDoneSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    int taskID = taskSpawn("tSendData", 100, 0, 2000, (FUNCPTR) sendPacket, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
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
