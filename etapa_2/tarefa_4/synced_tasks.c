#include <semLib.h>
#include <stdio.h>
#include <taskLib.h>
#include <vxWorks.h>

SEM_ID syncSemaphore;

// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void periodicTask1()
{
    for(char i='a';i<='z';i++)
    {  
        semTake(syncSemaphore, WAIT_FOREVER);
        logMsg("%c ", i);
        fflush(stdout); // Força mensagem de Log aparecer no kernel imediatamente
        
        semGive(syncSemaphore); // Envia sinal para semaforo
        taskDelay(50); // Delay para permitir que a task 2 termine execução
    }
}

void periodicTask2()
{
    for(char i='A';i<='Z';i++)
    {  
        semTake(syncSemaphore, WAIT_FOREVER);
         logMsg("%c ", i);
        fflush(stdout); // Força mensagem de Log aparecer no kernel imediatamente
        
        semGive(syncSemaphore); // Envia sinal para semaforo
        taskDelay(51); // Delay para permitir que a task 1 termine execução
    }
}

int main()
{
    // Cria uma "semaforo" binario
    syncSemaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);

    // Spawn tasks periodicas
    taskSpawn("Task1", 100, 0, 10000, (FUNCPTR)periodicTask1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    taskSpawn("Task2", 101, 0, 10000, (FUNCPTR)periodicTask2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    // Loop infinito para a função main
    while (1)
    {
        taskDelay(1000);
    }
}
