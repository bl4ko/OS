#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    /* -- Basic pipe example. -----------------------------------
        we have to open intermediary file with (pipe) returning two file descriptors 
        fd[0] -> read, fd[1] --> write
    */
    printf("-- Example 1: sending int x through pipe ------------------------------------------------------------------------------\n");
    int fd[2];
    // 1. Pipe creation
    if (pipe(fd) == -1) { perror("pipe"); return 1; }
    int pid = fork();
    if (pid == 0) {
        close(fd[0]); /* We have to always close the pipe */
        int x;
        printf("PID[%d]; Input a number: ", getpid());
        scanf("%d", &x);
        // 2. Write to pipe
        if (write(fd[1], &x, sizeof(int)) == -1) { perror("write"); return 2;}
        close(fd[1]); /* We have to always close the pipe */
        exit(0);
    } else {
        close(fd[1]);
        int x;
        // 3. Read from pipe
        if (read(fd[0], &x, sizeof(int)) == -1) { perror("read"); return 3; }
        close(fd[0]);
        printf("PID[%d]: got from childPID[%d] x=%d\n", getpid(), pid, x);
    }

    sleep(2);


    printf("\n\n-- Example 2: Each process calculates half of the sum of an array -----------------------------------------------------\n");
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8}; int arrSize = sizeof(arr) / sizeof(int); printf("arr = {1, 2, 3, 4, 5, 6, 7, 8}\n");
    if (pipe(fd) == -1) perror("pipe");
    pid = fork(); 
    if (pid == 0) {
        close(fd[0]);
        int childSum = 0;
        for (int i = 0; i < arrSize/2; i++) childSum += arr[i];
        printf("Child[%d]: sum({1, 2, 3, 4}) = %d\n", getpid(), childSum);
        write(fd[1], &childSum, sizeof(childSum));
        printf("Write to pipe: childSum=%d", childSum);
        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        int parentSum = 0;
        int childSum;
        for (int i = arrSize/2; i < arrSize; i++) parentSum += arr[i];
        childSum = read(fd[0], &childSum, sizeof(childSum));
        printf("Read from pipe: childSum = %d\n", childSum);
        close(fd[0]);
        printf("Parent[%d]: sum({5, 6, 7, 8}) = %d\n", getpid(), parentSum);
        int completeSum = childSum + parentSum;
        printf("Complete sum = %d\n", completeSum);
        wait(NULL);
    }
    return 0;
}
