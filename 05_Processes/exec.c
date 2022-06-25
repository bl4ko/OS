#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pid = fork();
    if (pid < 0) { return 1; }
    if (pid == 0) {
        // execlp(script_name, arg0, ..., argN, NULL); 
        execlp("ping", "ping", "-c", "3", "google.com", NULL);
        /* Code after here doesn't execute, unless there is an error */
    } else {
        wait(NULL);
        printf("Some post processing goes here\n");
    }
    return 0;
}
