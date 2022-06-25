#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
    int pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (pid == 0) {
        while (1) {
            printf("Child: I am still alive and you can't stop me, muahaaha\n");
            usleep(50000);
        }
    }
    else {
        kill(pid, SIGSTOP); // stop child
        int t;
        do {
           printf("Time in seconds for execution: "); 
           scanf("%d", &t);
            if (t > 0) {
                kill(pid, SIGCONT); // continue child
                sleep(t);
                kill(pid, SIGSTOP); //  stop child
            }
        } while (t > 0);
        kill(pid, SIGKILL);
        wait(NULL);
    }
    return 0;
}
