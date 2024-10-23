#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>

// Funcao repeat que executa uma task
void repeat(void (*task)(void), int times) {
    for (int i = 0; i < times; i++) {
        task(); // Em VxWorks significa uma task independente no thread
    }
}

// Task com printf
void exampleTask(void) {
    printf("Hello World!\n");
}

// Task periodica
void callRepeatFunction(void) {
    int repeatCount = 5;  // Numero de vezes que a task ira repetir
    printf("Repetindo a task %d vezes...\n", repeatCount);
    repeat(exampleTask, repeatCount);  // Chama a funcao repeat
}

// Main function
int main(void) {
    taskSpawn("tCallRepeat", 100, 0, 2000, (FUNCPTR) callRepeatFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}
