# Sistemski klici

###  Error detection
- Koda zadnje napake
    - spremenljivka errno
- Izpis obvestila o napaki funkcija `perror()`

```bash
errno -l            # izpise vse kode napak
errno 42            # izpise: ENOMSG 42 No message of desired type
```
### Sistemske funkcije
1. Create a new file
    * int create(char* filename, mode_t mode)
        * mode: indicates permissions of a new file
        * returns: first unused file descriptor or -1 when error 

2. Open the file
    * int open(const char* Path, int flags);
        * O_RDONLY: read only
        * O_WRONLY: write only
        * O_RDWR: read and write
        * O_CREAT: create file if it doesn't exist
        * O_EXCL: prevent creation if it already exist
        * O_TRUNC: truncate regular writable file when opened
        * O_APPEND: append mode
        * https://man7.org/linux/man-pages/man2/open.2.html

3. Close: tellls the operating system you are done with a file descriptor and close the file which pointed by fd.
    * int close(int fd)
    * returns: 0 on success, -1 on error

4. Read: REads cnt bytes of input into the memory area indicated by buf
    * size_t read(int fd, void* buf, size_t cnt)
    * cnt length of the buf
    * return: 
        * return Number of bytes read on success
        * return 0 on reaching end of file
        * reutnr -1 on error
        * return -1 on signal interrupt

5. Write: writes cnt bytes from but to the file
    * size_t write(int fd, void* buf, size_t cnt)
    * return:
        * Number of bytes on success
        * 0 on reaching end of file
        * -1 on error
        * -1 on signal interrupt

- Systemcalls example of `cpcat`

```bash
#include <unistd.h>
#include <errno.h>          // error number: errno 
#include <fcntl.h>          // Flags for open
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syscall.h"        // BUFSIZ


void cpCat(const char* progName, const char* srcPath, const char* destPath);
void writeErorr(int errNum, const char* progName); 


int main(int argc, char const *argv[]) {
    switch (argc) {
        case 1:
            cpCat(argv[0], "-", "-");
            break;
        case 2:
            cpCat(argv[0], argv[1], "-");
            break;
        case 3:
            cpCat(argv[0], argv[1], argv[2]);
            break;
        default:
            write(2, "Too many arguments given.\n", 26);
            break;
    }
    return 0;
}

void cpCat(const char* progName, const char* srcPath, const char* destPath) {

    // -- 1. Opening files -------------------------------------------
    int srcDesc = strcmp(srcPath, "-") == 0 ? 0 : open(srcPath, O_RDONLY);
    if (srcDesc < 0) 
        writeErorr(errno, srcPath);

    int destDesc = strcmp(destPath, "-") == 0 ? 1 : open(destPath, O_WRONLY);
    if (destDesc < 0) 
        writeErorr(errno, destPath);

    // -- 2. Copying data ---------------------------------------------
    char buff[BUFSIZ];
    int n_read;
    while ((n_read = read(srcDesc, buff, BUFSIZ)) > 0) {
        if (write(destDesc, buff, n_read) < 0) 
            writeErorr(errno, destPath);
        n_read = 0;
    }
    if (n_read < 0) {
        writeErorr(errno, srcPath);
    }
    
    // -- 3. Closing files --------------------------------------------
    if (close(srcDesc) < 0) 
        writeErorr(errno, srcPath);
    
    if (close(destDesc) < 0) 
        writeErorr(errno, destPath);
}

void writeErorr(int errNum, const char* progName) {
    perror(progName);
    exit(errNum);
}
```
### syscall()
But what is the function `write` doing? Going one level deeper, we can call the `syscall` function with the same arguments, plus the argument SYS_write specifying the system call number:
```c
#include <unistd.h>
#include <sys/syscall.h>
int main(void) {
  syscall(SYS_write, 1, "hello, world!\n", 14);
  return 0;
}
```