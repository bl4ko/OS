#include <unistd.h>

int main(int argc, char const *argv[])
{
    write(1, "Juhuhu, pomlad je tu.", 21);
    write(2, "Zima prihaja!", 13);
    return 0;
}
