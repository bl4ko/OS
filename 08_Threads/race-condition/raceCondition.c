#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int mails = 0;
/* We see that both threads execute at the same time */
void* routine() {
    for (int i = 0; i < 10000; i++) {
        mails++;
        // load mails
        // increment mails
        // store
    }
}

// compile with gcc -pthread
int main(int argc, char const *argv[])
{
    pthread_t t1, t2; 
    if (pthread_create(&t1, NULL, &routine, NULL) != 0) return 1;
    if (pthread_create(&t2, NULL, &routine, NULL) != 0) return 2;
    if (pthread_join(t1, NULL) != 0) return 3; /* Thread has finished execution */
    if (pthread_join(t2, NULL) != 0) return 4; /* Thread has finished execution */

    /* Expected value is 20000 */
    printf("Number of mails: %d\n", mails);
    /* What has happened? */
    /* Race condition: "./images/souporaba-problem.png" */
    return 0;
}
