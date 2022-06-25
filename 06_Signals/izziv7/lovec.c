#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/time.h>

void handle_sigterm(int sig);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);
void handle_sigint(int sig);
void catch_zombie(int sig);

int energy = 0;
char znak = '.';

int main(int argc, char *argv[])
{
    energy = argc == 2 ? atoi(argv[1]) : 42;

    int pid = fork();
    if (pid == 0) {
        char ppid[10];
        sprintf(ppid, "%d", getppid());
        printf("ppid: %s\n", ppid);
        execl("./sigsend.sh", "./sigsend.sh", ppid, (char *) 0);
        perror("execvp");
        return 1;
    }
    else {
        signal(SIGTERM, &handle_sigterm);
        signal(SIGUSR1, &handle_sigusr1);
        signal(SIGUSR2, &handle_sigusr2);
        signal(SIGINT, &handle_sigint);
        signal(SIGCHLD, &catch_zombie);

        printf("My PID: %d\n", getpid()); 
        while (energy > 0) {
            sleep(1);
            putchar(znak);
            fflush(stdout);
            energy-=1;
        }
        kill(pid, SIGKILL); // kill the child
        wait(NULL);
        printf("\nOut of energy. Aggghhhhhrrr\n");
    }
    return 0;
}

void handle_sigterm(int sig) {
    energy += 10;
    printf("Yahoo! Bonus energy (%d)\n.", energy);
}

void handle_sigusr1(int sig) {
    if (znak == '.') znak = '*';
    else znak = '.';
}

void handle_sigusr2(int sig) {
   int pid = fork(); 
   if (pid == 0) {
       sleep((energy % 7) + 1);
       int exit_status = (42 * energy) % 128;
       printf("Child exit with status: %d\n", exit_status);
       exit(exit_status);
    } else {
        printf("Forked child %d\n", pid);
   }
}

void catch_zombie(int sig) {
    int status = 0;
    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Zombie with status %d cought!\n", exit_status);
            return;
        }
    }
}

void handle_sigint(int sig) {
    printf("Child wanted to kill me with SIGINT\n");
    printf("But I'm two strong so I won't die\n");
}
