#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
   Execute: cat /etc/passwd | cut -d -f7 | sort -u
*/

int main(int argc, char* argv[]) {
    int fd1[2];
    pipe(fd1);
    if (fork() == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]); close(fd1[1]);
        execlp("cat", "cat", "/etc/passwd", NULL);
    } 

    int fd2[2];
    pipe(fd2);
    if (fork() == 0) {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]); close(fd1[1]); close(fd2[0]); close(fd2[1]);
        execlp("cut", "cut", "-d:", "-f1", NULL);
    }
    close(fd1[0]); close(fd1[1]);

    if (fork() == 0) {
        dup2(fd2[0], 0);
        close(fd2[0]); close(fd2[1]);
        execlp("sort", "sort", "-u", NULL);
    }
    close(fd2[0]); close(fd2[1]);
    for (int i = 0; i < 3; i++) wait(NULL);
    return 0;
}

