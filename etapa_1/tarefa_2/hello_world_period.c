#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>
#include <time.h>

#define TASK_PRIORITY 100    // Define prioridade da task
#define TASK_STACK_SIZE 1000 // Define stack size da task
#define LOOP_DELAY 1000      // Delay em milisegundos

// Task function
void helloWorldTask() {
    int i = 0;
    while (i < 10) {
        printf("Hello, World!\n");
        taskDelay(LOOP_DELAY);
        i++;
    }
}

int main() {
    // Criando a task
    TASK_ID taskId = taskSpawn("tHelloWorld", 
                                TASK_PRIORITY, 
                                0, 
                                TASK_STACK_SIZE, 
                                (FUNCPTR)helloWorldTask, 
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (taskId == ERROR) {
        printf("Error creating task\n");
    }

    return 0;
}
