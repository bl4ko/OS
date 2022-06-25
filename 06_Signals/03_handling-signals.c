#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void handle_sigtstp(int sig) {
    printf("Stop not allowed\n");
}

void handle_sigcont(int sig) {
    printf("Input number: ");
    fflush(stdout);
}

int main(int argc, char const *argv[])
{
    // when we press C-z -> stop not allowed
    // Longer way
    struct sigaction sa;
    sa.sa_handler = &handle_sigcont;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCONT, &sa, NULL); 

    // Shorter way (this version varies across UNIX versions)
    // // Avoid its use using sigaction
    // signal(SIGTSTP, &handle_sigtstp);

    int x;
    printf("Input a number: ");
    scanf("%d", &x);
    printf("Result %d * 5 = %d\n", x, x * 5);
    return 0;
}
