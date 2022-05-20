#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int mails = 0;
int N = 0;
int M = 5;

void* routine(void* arg) {
    for (int i = 0; i < M; i++) {
        printf("thrad[%d]=%d\n",*(int *)arg, i);
        sleep(1);
    }    
    free(arg);
}

int main(int argc, char const *argv[])
{
    if (argc <= 1) return 0;
    int N = atoi(argv[1]);
    if (argc == 3) {
        M = atoi(argv[2]);
    }
    pthread_t th[N];
    for (int i = 0; i < N; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        pthread_create(&th[i], NULL, &routine, a);
        printf("Thread %d has started\n", i);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(th[i], NULL);
        printf("Thread %d has finished\n", i);
    }
    return 0;
}


