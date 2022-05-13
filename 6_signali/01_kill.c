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
        sleep(1);
        kill(pid, SIGKILL); // ugasnemo otroka
        wait(NULL);
    }
    return 0;
}
