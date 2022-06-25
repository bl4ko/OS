#include <unistd.h> // execvp
#include <stdio.h> // fprintf, 
#include <stdlib.h> //
#include <errno.h> // errno
#include <fcntl.h> // flags for open O_CREAT
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc <= 3) {
        fprintf(stderr, "Premalo vhodnih argumentov, vsaj 3: <vhod> <izhod> <prog> <args...>");
        return 1;
    }

    // Open input file
    int in = strcmp(argv[1], "-") == 0 ? 0 : open(argv[1], O_RDONLY);
    if (in < 0) {
        int err_code = errno;
        perror("open");
        exit(errno);
    }

    // Open output file
    int out = strcmp(argv[2], "-") == 0 ? 1 : open(argv[2], O_CREAT | O_WRONLY);
    if (out < 0) {
        int err_code = errno;
        perror("open");
        exit(errno);
    }

    // Spawn the child process
    int pid = fork();
    if (pid == 0) {

        dup2(in, 0);            // replcae stdin wtih the input file
        dup2(out, 1);           // replace stdout with output file
        // Newly spawned child Process.
        if (execvp(argv[3], &argv[3]) < 0) {
            int error_code = errno;
            perror("execvp");
            exit(errno);
        }
    }
    else {
        // Old parent process. The C program will come here
    }

    return 0;
}
