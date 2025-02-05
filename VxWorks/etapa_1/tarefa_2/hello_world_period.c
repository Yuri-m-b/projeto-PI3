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


// Task periodica
void callRepeatFunction(void) {
    int repeatCount = 5;  // Numero de vezes que a task ira repetir
    for (int i = 0; i < repeatCount; i++) {
        logMsg("Hello World!\n");
        taskDelay(100);
    }
    logMsg("Repetindo a task %d vezes...\n", repeatCount);
}

// Main function
int main(void) {
    int taskId = taskSpawn("tCallRepeat", 100, 0, 2000, (FUNCPTR) callRepeatFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    while(1)
    {
        taskDelay(100);
    }
}
