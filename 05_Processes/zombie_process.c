/*
A zombie process is a process that has completed execution (via the exit system call), 
but still has an entry in the process table. This occurs for the child process, where
the entry is still needed to allow the parent process to read its child's exit stauts.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int pid = fork();
    if (pid > 0) {
        // parent process
        sleep(10);
    } else {
        // child exits, but father is sleeping
        printf("Child pid: %d\n", getpid());
        exit(0);
    }
    return 0;
}
