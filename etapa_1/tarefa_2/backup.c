#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>
#include <time.h>

#define TASK_PRIORITY 100    // Define prioridade da task
#define TASK_STACK_SIZE 1000 // Define stack size da task
#define LOOP_DELAY 100      // Delay em milisegundos

// Task function
void helloWorldTask() {
        printf("Hello, World!\n");
}

int main() {
    // Criando a task
    while(1){
        TASK_ID taskId = taskSpawn("tHelloWorld", 
                                TASK_PRIORITY, 
                                0, 
                                TASK_STACK_SIZE, 
                                (FUNCPTR)helloWorldTask, 
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        taskDelay(LOOP_DELAY);
    }


    return 0;
}
