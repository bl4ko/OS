#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

/* 
    (ls | grep xyz >> bla.txt) & 
*/

int main(int argc, char const *argv[])
{
    if (fork() == 0) {
        int fd[2];
        pipe(fd);

        sleep(5);
        if (fork() == 0) {
            dup2(fd[1], 1); 
            close(fd[0]); close(fd[1]);
            execlp("ls", "ls", NULL);
            perror("execlp"); exit(42);
        } 

        if (fork() == 0) {
            int f = open("bla.txt", O_WRONLY | O_CREAT | O_APPEND, 0644); // O_WRONLY: write only, O_CREAT: create file if it doesnt' exist
            dup2(fd[0], 0);
            dup2(f, 1); // preusmerimo izhod na bla.txt
            close(fd[0]); close(fd[1]); close(f);
            execlp("grep", "grep", "primer", NULL);
            perror("execlp"); exit(42);
        }

        close(fd[0]); close(fd[1]);
        wait(NULL);
        wait(NULL);
    }
    // ne powaitamo: zelimo v ozadju (nad lupina ne caka)
    return 0;
}
