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

void exampleTask(void) {
    logMsg("Hello World!\n");
    taskDelay(100);
}

// Função para criar nova tarefa
void repeat(void (*task)(void), int times) {
    for (int i = 0; i < times; i++) {
        taskSpawn("tExampleTask", 100, 0, 2000, (FUNCPTR)task, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void callRepeatFunction(void) {
    int repeatCount = 5;
    logMsg("Repetindo a task %d vezes...\n", repeatCount);
    repeat(exampleTask, repeatCount);
}

int main(void) {
    taskSpawn("tCallRepeat", 100, 0, 2000, (FUNCPTR)callRepeatFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}
