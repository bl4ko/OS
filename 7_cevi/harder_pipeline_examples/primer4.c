#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // for open and constants O_WRONLY...

/*
    ls | cat > 2
*/

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]); close(fd[1]);
        execlp("ls", "ls", "-la", NULL);
    } 

    if (fork() == 0) {
        int f = open("2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd[0], 0);
        dup2(f, 1);
        close(fd[0]); close(fd[1]); close(f);
        execlp("cat", "cat", NULL);
    }

    close(fd[0]); close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}