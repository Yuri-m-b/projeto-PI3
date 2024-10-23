#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>

// Definição da função que será executada pela task 
void helloWorldTask()
{
    printf("Hello, World!\n");
}

// Função principal para criar a task 
void createHelloWorldTask()
{
    // Cria uma task com a prioridade 100 e um tamanho de pilha de 2000 bytes
    int taskId = taskSpawn(
        "tHelloWorld",   // Nome da task
        100,             // Prioridade da task (100 é uma prioridade média)
        0,               // Opções da task (0 para opções padrão)
        2000,            // Tamanho da pilha da task
        (FUNCPTR)helloWorldTask,  // Função que será executada pela task
        0, 0, 0, 0, 0,   // Argumentos para a função (até 10 argumentos permitidos)
        0, 0, 0, 0, 0);

    // Verifica se a task foi criada com sucesso 
    if (taskId == ERROR)
    {
        printf("Erro ao criar a task!\n");
    }
    else
    {
        printf("Task criada com sucesso, ID: %d\n", taskId);
    }
}

// Função principal
int main()
{
    printf("Iniciando a aplicação...\n");
    createHelloWorldTask();

    return 0;
}
