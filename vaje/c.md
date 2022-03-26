# Sistemski klici

### Obdelava napak
- Koda zadnje napake
    - spremenljivka errno
- Izpis obvestila o napaki funkcija `perror()`

```bash
errno -l            # izpise vse kode napak
errno 42            # izpise: ENOMSG 42 No message of desired type
```
### Sistemske funkcije
```c
/*
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
*/
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