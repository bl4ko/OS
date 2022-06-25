#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_PIPE_NAME_LENGTH 100
#define IN_FIFO "proces.cev"
#define OUT_FIFO "streznik.cev"

void throw_error(int line_num, int err_num, char* progName);
int beriIzCevi();
void pisiNaCev(char* text[], int numWords);

char buffer[BUFSIZ];

int main() {
    if (mkfifo(IN_FIFO, 0644) < 0) {
        throw_error(__LINE__, errno, "mkfifo");
    }
    // connect 
    char* text0[] = {"connect", "proces.cev"};
    pisiNaCev(text0, 2);
    // write
    char* text1[] = {"add", "1", "5", "4"};
    pisiNaCev(text1, 4);
    // read
    int rez = beriIzCevi();
    // disconnect
    char* text2[] = {"disconnect"};
    pisiNaCev(text2, 1);

    printf("3 + 5 + 8 = %d\n", rez);
}

void pisiNaCev(char* text[], int numWords) {
    int out_fd = open(OUT_FIFO, O_WRONLY);
    for (int i = 0; i < numWords; i++) {
        if (write(out_fd, text[i], sizeof(char) * (strlen(text[i])+1)) == -1) {
            throw_error(__LINE__, errno, "write");
        }
    }
    write(out_fd, "\n", sizeof(char));
    close(out_fd);
}

int beriIzCevi() {
    int fd_in = open(IN_FIFO, O_WRONLY);
    char buffer[BUFSIZ];
    int nread = read(fd_in, buffer, sizeof(char) * BUFSIZ);
    if (nread == -1) throw_error(__LINE__, errno, "nread");
    close(fd_in);
    return atoi(buffer);
}


void throw_error(int line_num,  int err_num, char* progName) {
    printf("%s[%d]: ", progName, line_num);
    perror("");
    exit(err_num);
}

