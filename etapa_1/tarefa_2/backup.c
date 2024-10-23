#include <stdio.h>
#include <vxWorks.h>
#include <taskLib.h>

// Custom repeat function that executes a task multiple times
void repeat(void (*task)(void), int times) {
    for (int i = 0; i < times; i++) {
        task();
        //taskDelay(10);
    }
}

// Example task to be repeated
void exampleTask(void) {
    printf("Task executed!\n");
}

// Function that calls the repeat() function
void callRepeatFunction(void) {
    int repeatCount = 5;  // Number of times to repeat the task
    printf("Repeating the task %d times...\n", repeatCount);
    repeat(exampleTask, repeatCount);  // Call repeat with exampleTask
}

// Main function to start execution
int main(void) {
    taskSpawn("tCallRepeat", 100, 0, 2000, (FUNCPTR) callRepeatFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}
