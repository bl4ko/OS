#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_PIPE_NAME_LENGTH 100
#define IN_FIFO "streznik.cev"

void throw_error(int line_num, int err_num, char* progName);


int main() {
    if (mkfifo(IN_FIFO, 0644) < 0) {
        throw_error(__LINE__, errno, "mkfifo");
    }

    char buffer[BUFSIZ];
    char words[100][20];
    char* OUT_FIFO = malloc(sizeof(char) * MAX_PIPE_NAME_LENGTH);
    int fd_in;
    FILE* outfifo_file;
    while (1) {
        if ((fd_in = open(IN_FIFO, O_RDONLY)) == -1) {
            throw_error(__LINE__, errno, "open");
        }
        
        int nread = read(fd_in, buffer, BUFSIZ);
        if (nread == -1) {
            throw_error(__LINE__,errno,"read");
        }

        printf("nread: %d\n", nread); fflush(stdout);

        if (close(fd_in) < 0) {
            throw_error(__LINE__, errno, "close");
        }

        for (int i = 0; i < nread; i++) {
            printf("buff[%d]=%c\n", i, buffer[i]); fflush(stdout);
        }

        int num_words = 0;
        printf("Scanning: \n"); fflush(stdout);
        int nmatched;
        int stPrebranih = 0;
        while ((nmatched = sscanf(&buffer[stPrebranih], "%s", words[num_words])) > 0) {
            printf("Server prebrano: %s\n", words[num_words]); fflush(stdout);
            stPrebranih += strlen(words[num_words]) + 1;
            num_words += 1; 
        }
        
        if (strcmp("connect", words[0]) == 0) {
            strcpy(OUT_FIFO, words[0]);
        } else if (strcmp("disconnect", words[0]) == 0) {
            exit(0); 
        } else if (strcmp("add", words[0]) == 0 || strcmp("mult", words[0]) == 0) {
            int res = 0;
            for (int i = 1; i < num_words; i++) {
                if (strcmp(words[0], "add") == 0) 
                    res += atoi(words[i]);
                else res *= atoi(words[i]);
            }
            int out_fd = open(OUT_FIFO, O_WRONLY);
            write(out_fd, "%d\n", res);
            close(out_fd);
        }
    }
    free(OUT_FIFO);
}


void throw_error(int line_num,  int err_num, char* progName) {
    char err_buf[128];
    sprintf(err_buf, "%s -> %d", progName, line_num);
    perror(err_buf);
    exit(err_num);
}

