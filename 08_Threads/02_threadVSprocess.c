#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

int x = 2;
// Threads: same address space (share file handlers, all the system resources are shared)
// Multiple processes: each have different address space
void* routine() {
    printf("First Thread PID[%d]: x++\n", getpid());
    x++;
    sleep(2);
    printf("First  Thread:  x=%d\n", x);
}

void* routine2() {
    printf("Second Thread PID[%d]: x+=2\n", getpid());
    x+=2;
    sleep(2);
    printf("Second Thread:  x=%d\n", x);
}

int main(int argc, char const *argv[])
{
    printf("-- Testing threads --------------------------------------\n");
    printf("x=%d\n", x);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("-- Testing processes --------------------------------------\n");
    printf("x=%d\n", x);
    int pid = fork();
    if (pid == 0) {
        printf("Process PID[%d] x++\n", getpid());
        x++;
    }
    sleep(2);
    printf("Process PID[%d] x: %d\n", getpid(), x);
    if (pid != 0) { wait(NULL); }
    return 0;
    return 0;
}
