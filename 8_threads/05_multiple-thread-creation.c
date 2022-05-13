#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex; /* basically a lock to the piece of code */

void* routine() {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, &routine, NULL);
        printf("Thread %d has started\n", i);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
        printf("Thread %d has finished\n", i);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
