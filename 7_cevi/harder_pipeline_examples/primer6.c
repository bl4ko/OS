#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // for open and constants O_WRONLY...

/*
    cat tocke | izracun.sh 2>log || mkdir FAIL &
*/

int main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
       dup2(fd[1], 1);
       close(fd[0]); close(fd[1]);
       execlp("cat", "cat", "tocke", NULL);
    }

    if (fork() == 0) {
        int f = open("log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd[0], 0);      
        dup2(f, 2);
        close(fd[0]); close(fd[1]); close(f);
    }

    close(fd[0], fd[1]);
    wait(NULL);
    int status;
    waitpid(-1, &status, 0);

    if (WIFEEXITED(status))
        // true if the process terminated normally by call to exit
        status = WEXITSTATUS(status);
    
    if (status != 0) {
        if (fork() == 0) {
            execlp("mkdir", "mkdir", "FAIL", NULL);        
        }
    }
    // ga ne pocakamo zaradi &
    return 0;
}
