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
        int err = execlp("ping", "ping", "-c", "3", "google.com", NULL);
        /* Code after here doesn't execute, unless there is an error */
        if (err < 0) {printf("Could not find program to execute.\n"); return 2; } 
    } else {
        int wstatus; // wait status
        wait(&wstatus);
        if (WIFEXITED(wstatus)) {
            int statusCode = WEXITSTATUS(wstatus);
            if (statusCode == 0) {
                printf("Success\n");
            } else {
                printf("Failure with status code %d\n", statusCode);
            }
        }
    }
    return 0;
}
