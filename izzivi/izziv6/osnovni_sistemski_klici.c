#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    printf("Kreiram otroka.\n");
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        int child_pid = getpid();
        printf("Hello this is child (PID=%d) speaking. I am going to execute the following command: %s\n", child_pid, argv[1]);
        execvp(argv[1], &argv[1]);
        exit(EXIT_FAILURE);
    } else {
        int father_pid = getpid();
        int status;
        int child_exit_status;
        if (waitpid(pid, &status, 0) == -1 ) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        // man wait
        if (WIFEXITED(status)) {
            child_exit_status = WEXITSTATUS(status);
        }
        printf("Hello this is father(PID=%d) speaking :\n", father_pid);
        printf("\tThe child (PID=%d) has exited with an exit status of %d\n", pid, child_exit_status);
    }
    exit(EXIT_SUCCESS);
}