#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>

// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

// Funcao para repetir a tarefa
void repeat(void (*task)(void), int times) {
    for (int i = 0; i < times; i++) {
        task(); // Em VxWorks significa uma task independente no thread
    }
}

// Task com printf
void exampleTask(void) {
    logMsg("Hello World\n");
}

// Task periodica
void callRepeatFunction(void) {
    int repeatCount = 5;  // Numero de vezes que a task ira repetir
    logMsg("Repetindo a task %d vezes...\n", repeatCount);
    repeat(exampleTask, repeatCount);  // Chama a funcao repeat
}

// Main function
int main(void) {
    taskSpawn("tCallRepeat", 100, 0, 2000, (FUNCPTR) callRepeatFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}
