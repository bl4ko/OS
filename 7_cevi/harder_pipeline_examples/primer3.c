#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // for open and constants O_WRONLY...

/* 
    ls | cut -d: -f2 | wc -l
*/

int main() {
    int fd[2][2]; // two pipes
    pipe(fd[0]);
    if (fork() == 0) {
        dup2(fd[0][1], 1);
        close(fd[0][0]); close(fd[0][1]);
        execlp("ls", "ls", NULL);
    }

    pipe(fd[1]);
    if (fork() == 0) {
        dup2(fd[0][0], 0);
        dup2(fd[1][1], 1);
        close(fd[0][0]); close(fd[0][1]); close(fd[1][0]); close(fd[1][1]);
        execlp("cut", "cut", "-d:", "-f2", NULL);
    }

    close(fd[0][0]); close(fd[0][1]);

    if (fork() == 0) {
        dup2(fd[1][0], 0);
        close(fd[1][0]); close(fd[1][1]);
        execlp("wc", "wc", "-l", NULL);
    }

    close(fd[1][0]); close(fd[1][1]);
    for (int i = 0; i < 3; i++) wait(NULL);
    return 0;  
}


