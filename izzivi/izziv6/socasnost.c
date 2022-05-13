#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DURATION 42

int main(int argc, char* argv[]) {
    // int i, pid;
    int N = 10;
    if (argc == 2) {
        N = atoi(argv[1]);
    }

    time_t duration = time(NULL) + DURATION;

    int pid, i; 
    for (i = 0; i < N; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            char izpis = 'A' + i;
            while (time(NULL) < duration) {
                printf("%c\n", izpis);
                sleep(i + 1);
            }
            exit(i);
        }
    }

    for (i = 0; i < N; i++) {
        int status = 0;
        pid_t childpid = wait(&status);
        printf("Parent knows child %d is finished.\n", (int)childpid);
    } 
    exit(42);
}