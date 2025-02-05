#include <vxWorks.h>
#include <taskLib.h>
#include <stdio.h>

#define MAX_ITERATION 4

// Funcao de log
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void workerTask(int id) {
    logMsg("Iteration: %d, ThreadId: %d\n", id, taskIdSelf());
}

int main() {
    int taskIds[MAX_ITERATION];
    for (int i = 0; i < MAX_ITERATION; i++) {
        // Cria uma nova task para cada iteração
        taskIds[i] = taskSpawn("tWorker", 100, 0, 4096, (FUNCPTR)workerTask, i, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        taskDelay(100);
    }

    logMsg("Task concluidas.\n");
    return 0;
}
