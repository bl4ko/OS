#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // for open and constants O_WRONLY...

/*
    cat f.txt 2> f.err | wc -l >> b.txt; cat b.txt
*/

int main() {
    int fd1[2];
    pipe(fd1);

    if (fork() == 0) {
        int f_err = open("f.err", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd1[1], 1);
        dup2(f_err, 2);
        close(fd1[0]); close(fd1[1]); close(f_err);
        execlp("cat", "cat", "f.txt", NULL);
    }

    if (fork() == 0) {
        int f = open("b.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd1[0] ,0);
        dup2(f, 1);
        close(fd1[0]); close(fd1[1]); close(f);
        execlp("wc", "wc", "-l", NULL);
    }

    close(fd1[0]); close(fd1[1]);
    
    wait(NULL); wait(NULL); // powaitamo cevovod

    if (fork() == 0) {
        execlp("cat", "cat", "b.txt", NULL);
    }

    wait(NULL);

    return 0;
}
