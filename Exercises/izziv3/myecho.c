#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

int main(int argc, char const *argv[])
{
     for (int i = 0; i < argc; i++) {
        //  printf("%s\n", argv[i]);

        // write(1, argv[i], strlen(argv[i]));
        // write(1, "\n", 1);

        syscall(SYS_write, 1, argv[i], strlen(argv[i]));
    }


    return 0;
}
