#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>
#include <time.h>
#include <semLib.h>

#define TASK_PRIORITY 100    // Define prioridade da task
#define TASK_STACK_SIZE 1000 // Define stack size da task
#define LOOP_DELAY 100      // Delay de acordo com os ticks.
SEM_ID taskDoneSem; // "Semaforo" para sinal de conclusão da task


// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}


// Task function
void helloWorldTask() {
    int i = 0;
    while (i < 10) {
        logMsg("Hello, World!\n");
        taskDelay(LOOP_DELAY);
        i++;
    }
    semGive(taskDoneSem);
}

int main() {
    // Cria uma "semaforo" binario
    taskDoneSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

    // Criando a task
    int taskID = taskSpawn("tHelloWorld", 
                                TASK_PRIORITY, 
                                0, 
                                TASK_STACK_SIZE, 
                                (FUNCPTR)helloWorldTask, 
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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
