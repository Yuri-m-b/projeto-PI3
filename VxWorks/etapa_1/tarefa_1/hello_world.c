#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>

// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}


// Definição da função que será executada pela task 
void helloWorldTask()
{
    logMsg("Hello, World!\n");
}

// Função principal
int main()
{
    printf("Iniciando a aplicação...\n");
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
        logMsg("Erro ao criar a task!\n");
    }
    else
    {
        logMsg("Task criada com sucesso, ID: %d\n", taskId);
    }
    taskClose(taskId);

    return 0; 
}

