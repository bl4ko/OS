#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 18 
#define MAX_USERS 4 // maximal number of users logined at the same time

sem_t* semaphore; 

void* routine(void* args) {
    printf("(%d) Waiting in the login queue\n", *(int*) args);
    sem_wait(semaphore);
    printf("(%d) Logged in\n", *(int*) args);
    sleep(rand() % 5 + 1);
    printf("(%d) Logged out\n", *(int*) args);
    sem_post(semaphore);
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    if ((semaphore = sem_open("./semaphore", O_CREAT, 0644, MAX_USERS)) == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    };
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    if (sem_close(semaphore) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink("./semaphore") == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
    puts("Done");
    exit(EXIT_SUCCESS);
    return 0;
}


