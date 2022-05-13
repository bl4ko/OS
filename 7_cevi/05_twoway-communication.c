#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int p1[2];
    if (pipe(p1) == -1) { return 1; }
    int pid = fork();
    if (pid == -1) { return 2; }
    if (pid == 0) {
        int x; 
        if (read(p1[0], &x, sizeof(x)) == -1) { return 3; }
        printf("Received %d\n", x);
        x *= 4;
        if (write(p1[1], &x, sizeof(x)) == -1) { return 4; }
        printf("Wrote %d\n", x);
    } else {
        // Parent process
        srand(time(NULL));
        int y = rand() % 10;
        if (write(p1[1], &y, sizeof(y)) == -1) { return 5; }
        printf("Wrote %d\n", y);
        if (read(p1[0], &y, sizeof(y)) == -1) { return 6; }
        printf("Result is %d\n", y);
    }
    return 0;
}