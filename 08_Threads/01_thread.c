#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

/* We see that both threads execute at the same time */
void* routine() {
    printf("Test from threads\n");
    sleep(3);
    printf("Ending thread");
}

// compile with gcc -pthread
int main(int argc, char const *argv[])
{
    pthread_t t1, t2; 
    if (pthread_create(&t1, NULL, &routine, NULL) != 0) return 1;
    if (pthread_create(&t2, NULL, &routine, NULL) != 0) return 2;
    if (pthread_join(t1, NULL) != 0) return 3;
    if (pthread_join(t2, NULL) != 0) return 4;
    return 0;
}
