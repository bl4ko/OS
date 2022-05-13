#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

// FIFO: file type to which you can read or write from any process.
// we can use it as a pipe: "named pipe"

int main(int argc, char const *argv[])
{
    if (mkfifo("myfifo1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    int fd = open("myfifo1", O_WRONLY); // proces caka da drugi proces odpre drugi konec cevi (za reading) -> nenadaljuje do takrat
    int x = 97;
    write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}
