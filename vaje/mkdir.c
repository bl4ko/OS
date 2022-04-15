#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    if (mkdir(argv[1], S_IRWXU) < 0) {
        printf("%d", errno);
        printf("%d", mkdir(argv[1], S_IRWXU));
        close(2);
        int koda = errno;
        perror(argv[0]); // Podamo ime nasega programa
        exit(koda); // ta errno ni izpisan od mkdir-ja ampak od perrorja (problem)
    };
    write(1, "Juhuhu\n", 7);
    exit(0);
}