/* mutex = lock around a section of code */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex; /* basically a lock to the piece of code */

/* We prevent race condition */
void* routine() {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

// compile with gcc -pthread
int main(int argc, char const *argv[])
{
    pthread_t t1, t2, t3, t4; 
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&t1, NULL, &routine, NULL) != 0) return 1;
    if (pthread_create(&t2, NULL, &routine, NULL) != 0) return 2;
    if (pthread_create(&t3, NULL, &routine, NULL) != 0) return 3;
    if (pthread_create(&t4, NULL, &routine, NULL) != 0) return 4;
    if (pthread_join(t1, NULL) != 0) return 5; /* Thread has finished execution */
    if (pthread_join(t2, NULL) != 0) return 6; /* Thread has finished execution */
    if (pthread_join(t3, NULL) != 0) return 7; /* Thread has finished execution */
    if (pthread_join(t4, NULL) != 0) return 8; /* Thread has finished execution */

    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;
}
