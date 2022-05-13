#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
   Execute: ping -c 5 google.com | grep rtt 
*/

int main(int argc, char* argv[]) {
    int fd[2];
    pipe(fd);
    int pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1], 1);
        close(fd[0]); close(fd[1]);
        execlp("ping", "ping", "-c", "3", "google.com", NULL);
    } 

    waitpid(pid1, NULL, 0);
    
    int pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], 0);
        close(fd[0]); close(fd[1]);
        execlp("grep", "grep", "round-trip", NULL);
    }
    close(fd[0]); close(fd[1]);

    waitpid(pid2, NULL, 0);
    return 0;
}