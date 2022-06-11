#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int N = 100;
    if (argc > 1) {
        N = atoi(argv[1]); 
    }

    // Ustvarimo touch stevec.txt && echo "0" > stevec.txt
    if (fork() == 0) {
        int f = open("stevec.txt", O_CREAT | O_WRONLY, 0644);
        dup2(f,1);
        close(f);
        execlp("echo", "echo", "0", NULL);
    }
    wait(NULL);


    // kreairamo 2N procesov, vsak z enolicnim id-jem
    int id = -1;
    for (int i = 0; i < 2*N; i++) {
        int pid = fork();
        if (pid == 0) {
            id = i;
            break;
        }
    }
    
    srand(time(NULL));
    if (id != -1) {
        // 1. Vsak prebere vrednost stevca iz datoteke
        FILE* stevec_txt = fopen("stevec.txt", "r"); 
        int stevec;
        fscanf(stevec_txt, "%d", &stevec);
        fclose(stevec_txt);
        // 2. Izpise svojo oznako in prebrano vrednost
        printf("%s %d: prebrano %d\n", id%2==0 ? "Inc" : "Dec", id, stevec);
        // 3. Spremeni vrednost inc inkrementira +1, proces dec pa dekrementira -1
        if (id % 2 == 0)    stevec++;
        else                stevec--;
        // 4. Po zelji na tem mestu kratek sleep
        /* sleep(rand() % 3); */ 
        // 5. Zapise spremenjeno vrednost nazaj v datoteko
        stevec_txt = fopen("stevec.txt", "w"); 
        fprintf(stevec_txt, "%d", stevec);
        fclose(stevec_txt);
        // 6. Izpise svojo oznako in spremenjeno vrednost
        printf("%s %d: zapisano %d\n", id%2==0 ? "Inc" : "Dec", id, stevec);
        exit(0);
    }


    // Pocakamo vse procese da se zaklucijo
    for (int i = 0; i < 2*N; i++) {
        wait(NULL);
    }

    // Main program
    int stevec;
    FILE* stevec_txt = fopen("stevec.txt", "r"); 
    fscanf(stevec_txt, "%d", &stevec);
    fclose(stevec_txt);
    printf("Koncni stevec: %d\n", stevec);
    return 0;
}
