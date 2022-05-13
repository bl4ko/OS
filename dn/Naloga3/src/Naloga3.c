#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isspace(int c)
#include <fcntl.h> // debug open file
#include <sys/stat.h> // mkdir, stat (get file status)
#include <dirent.h>    // working with directory
#include <sys/errno.h>
#include <sys/stat.h> // file status

/* -- START-CONSTANTS ------------------------------------------------- */
#define MAX_SH_NAME_LEN 30     // maximal shell name length
#define MAX_TOKENS 50          // maximal number of input tokens
#define ANSI_COLOR_CYAN    "\x1b[36m" // CYAN color
#define ANSI_COLOR_MAGENTA "\x1b[35m" // MAGENTA
#define ANSI_COLOR_RESET   "\x1b[0m" // default color
#define ANSI_COLOR_BLUE    "\x1b[34m" // blue
#define MAX_PIPE_ARGS 30      // maximal number of args in a single pipe
#define MAX_NUMBER_OF_HARDLINKS 100
/* -- END-CONSTANTS --------------------------------------------------- */

/* -- START GLOBAL VARIABLES ------------------------------------------ */
char sh_name[MAX_SH_NAME_LEN];      // shell name:  sh_name>
char* tokens[MAX_TOKENS];           // tabela vhodnih tokenov
int token_count = 0;                // real: tokens.length
int exit_status = 0;                 // zadnji izhodni status (v odspredju) izvedenega ukaza
int saved_stdout = -1;              // Saved File descriptor for stdin
int saved_stdin = -1;               // Saved File descriptor for stdout
int bg = 0;                         // If a process is a background process
/* -- END GLOBAL VARIABLES -------------------------------------------- */

/* -- START FUNCTION PROTOTRYPES -------------------------------------- */
void throw_err(int line, int err_code);
void print_error(char* progName, int err_num);
int tokenize(char* line); // Razpoznavanje simbolov (branje vrstice)
int ptokenize(char* dst[], char* pline); // Tokenize pipeline
void name();              // nastavi ime lupine
void help();              // izpisi pomoc
void status();
void exit_custom_status();
void print();
void echo();
void pid();
void ppid();
void cutQuotes();
void dirchange();
void dirwhere();
void dirmake();
void dirremove();
void dirlist();
void linkhard();
void linksoft();
void linkread();
void linklist();
void _unlink();
void _rename();
void cpcat();
void pipes();
void zunanjiUkaz();
void pcpcat(char* ptokens[]); 
/* -- END FUNCTION PROTOTRYPES ---------------------------------------- */


/*
----- 1. REPL Read Eval & Print Loop ----------------------------------------------------------------------------------------------------------------------------------------------------
* Med interaktivnim (ročno vnašanje ukazov) in neinteraktivnim (skriptni način) načinom lahko ločimo s pomočjo funkcije isatty(1).  
* Če program zaženemo s presumeritvijo vhoda, gre za neinteraktivni način, npr. ./mysh <skripta.sh, sicer pa za interaktivnega. 
* Ukazno lupino sprogramiramo po principu repl:
    * v interaktvinem nacinu izpisemo povizvnik -- ime lupine in ">", npr "mysh>"
    * preberemo eno vrstcio s standardnega vhoda (READ)
    * jo izvedemo (EVAL)
    * izpisemo rezultat na standardni izhod (PRINT)
    * ponavljamo (LOOP)
*/

int main(int argc, char const *argv[])
{
    // int f = open("../../dn3-testi/vhodi/test-H.txt", O_RDONLY, 0644); /* debug */
    // dup2(f, 0); /* debug */
    // close(f); 
    // f = open("out.txt", O_WRONLY, 0644);
    // dup2(f, 1);
    // close(f);

    strcpy(sh_name, "mysh");

    while (1) {
        // isatty - test whether a file descriptor refers to a terminal
        // preverimo ali smo v interaktvinem/neinteraktivnem nacinu
        if (isatty(1)) printf("%s> ", sh_name);

        // Pocakamo na pritisk tipke enter='\n'
        // Nato preberemo eno vrstico s standardnega vhoda *READ*
        char* line = NULL; size_t size;
        int nread = getline(&line, &size, stdin);
        if (nread == -1) {
            // EOF or error
            break; 
        }
        if (!tokenize(line)) continue; // comment or empty line (only spaces)

        // Check if a Background Process
        bg = 0; // if a process is background = 1, else = 0
        if (tokens[token_count-1][0] == '&') {
            bg = 1;
            token_count-=1;
            int pid = fork();
            if (pid < 0) throw_err(__LINE__, errno);
            if (pid != 0) continue; // main process continue
        }

        int fd_in;  // dummy fd for new input 
        int fd_out; // dummy fd for new output
        for (int i = token_count-1; i >= 1; i--) {
            if (tokens[i][0] == '<') {
                // fprintf(stdout, "<%s --> ", &tokens[i][1]); fflush(stdout);
                fd_in = open(&tokens[i][1], O_RDONLY | O_CREAT, 0644);
                // fprintf(stdout, "fd:%d\n", fd_in); fflush(stdout);
                if (fd_in == -1) throw_err(__LINE__, errno);
                saved_stdin = dup(0);
                dup2(fd_in, 0); close(fd_in);
                token_count--;
            }
            else if (tokens[i][0] == '>') {
                // fprintf(stdout, ">%s --> ", &tokens[i][1]); fflush(stdout);
                fd_out = open(&tokens[i][1], O_WRONLY | O_CREAT, 0644);
                // fprintf(stdout, "fd:%d\n", fd_out); fflush(stdout);
                if (fd_out == -1) throw_err(__LINE__,errno);
                saved_stdout = dup(1);
                dup2(fd_out, 1); close(fd_out);
                token_count--;
            }
        }

        if (strcmp(tokens[0], "name") == 0) name();
        else if (strcmp(tokens[0], "help") == 0) help();
        else if (strcmp(tokens[0], "status") == 0) status();
        else if (strcmp(tokens[0], "exit") == 0) exit_custom_status();
        else if (strcmp(tokens[0], "print") == 0) print();
        else if (strcmp(tokens[0], "echo") == 0) echo();
        else if (strcmp(tokens[0], "pid") == 0) pid();
        else if (strcmp(tokens[0], "ppid") == 0) ppid();
        else if (strcmp(tokens[0], "dirchange") == 0) dirchange();
        else if (strcmp(tokens[0], "dirwhere") == 0) dirwhere();
        else if (strcmp(tokens[0], "dirmake") == 0) dirmake();
        else if (strcmp(tokens[0], "dirremove") == 0) dirremove();
        else if (strcmp(tokens[0], "dirlist") == 0) dirlist();
        else if (strcmp(tokens[0], "linkhard") == 0) linkhard();
        else if (strcmp(tokens[0], "linksoft") == 0) linksoft();
        else if (strcmp(tokens[0], "linkread") == 0) linkread();
        else if (strcmp(tokens[0], "linklist") == 0) linklist();
        else if (strcmp(tokens[0], "unlink") == 0) _unlink();
        else if (strcmp(tokens[0], "rename") == 0) _rename();
        else if (strcmp(tokens[0], "cpcat") == 0) cpcat();
        else if (strcmp(tokens[0], "pipes") == 0) pipes();
        else zunanjiUkaz();

        // stdout flushing
        fflush(stdout);

        // check if background process
        if (bg == 1) exit(0);

        // Repair standard descriptors (0, 1), if needed
        if (saved_stdin != -1) {
            dup2(saved_stdin ,0);
            saved_stdin = -1;
        }
        if (saved_stdout != -1) {
            dup2(saved_stdout, 1);
            saved_stdout = -1;
        }
    }
    return 0;
}

void zunanjiUkaz() {
    tokens[token_count] = NULL;
    int pid = fork();
    if (pid == 0) {
        execvp(tokens[0], tokens);
    }
    int wstatus; // wait status
    wait(&wstatus);
    if (WIFEXITED(wstatus)) {
        exit_status = WEXITSTATUS(wstatus);
        if (exit_status != 0) perror("zunanjiUkaz");
    }
}

// Line -> Token array[] 
int tokenize(char* line) {
    token_count = 0;
    int lineLength = strlen(line);
    int currTokenLength = 0;
    int quoteMode = 0;  // check if we are in quote mode " ... "
    char comment = 'Y';     // check if begins with #
    for (int i = 0; i < lineLength; i++) {
        // check if line is comment
        if (!isspace(line[i]) && comment == 'Y') {
            if (line[i] == '#') return 0;
            else comment = 'N';
        }  

        if (line[i] == '"') {
            quoteMode = (quoteMode + 1) % 2;
        }
        if (!isspace(line[i]) || quoteMode) {
            currTokenLength++;
        }
        else if (isspace(line[i]) && currTokenLength != 0) {
            line[i] = '\0';
            tokens[token_count] = &line[i] - currTokenLength;
            cutQuotes();
            token_count++;
            currTokenLength = 0;
        } 
    }
    if (currTokenLength > 0) {
        tokens[token_count] = &line[lineLength] - currTokenLength;
        token_count++;
    }
    return token_count > 0 ? 1 : 0;
}

/* Preprosti vgrajeni ukazi */
// "Ananaa k" --> Ananaa k
void cutQuotes() {
    if (tokens[token_count][0] == '"') {
        tokens[token_count][strlen(tokens[token_count])-1] = '\0';
        tokens[token_count] = tokens[token_count] + 1;
    }
}

int ptokenize(char* ptokens[], char* pline) {
    int ptoken_count = 0;
    int lineLength = strlen(pline);
    int currTokenLength = 0;
    for (int i = 0; i < lineLength; i++) {
        if (!isspace(pline[i])) {
            currTokenLength++;
        }
        else if (isspace(pline[i]) && currTokenLength != 0) {
            pline[i] = '\0';
            ptokens[ptoken_count] = &pline[i] - currTokenLength;
            ptoken_count++;
            currTokenLength = 0;
        } 
    }
    if (currTokenLength > 0) {
        ptokens[ptoken_count] = &pline[lineLength] - currTokenLength;
        ptoken_count++;
    }
    ptokens[ptoken_count] = NULL;
    return token_count > 0 ? token_count : -1;
}

// name ime - nastavi ime lupine ce imena ne podamo izpise ime lupine
void name() {
    if (token_count > 1) {  
        char* cpy = strcpy(sh_name, tokens[1]);  
        if (cpy == NULL) {
            print_error("name", errno);
        }
    }
    else {
        exit_status = printf("%s\n", sh_name);
        if (exit_status < 0) perror("name");
        exit_status = 0;
    }  
}

void help() {
    printf(ANSI_COLOR_BLUE "\nDOBRODOSLI V MIKRO LUPINI %s\n\n" ANSI_COLOR_RESET, sh_name);
    printf(ANSI_COLOR_CYAN "%-8s" ANSI_COLOR_MAGENTA "%-8s" ANSI_COLOR_RESET ": nastavi ime lupine, ce imena ne podamo, izpise ime lupine (privzeto ime je mysh)\n", "name", "<ime>"); 
    printf(ANSI_COLOR_CYAN "%-16s" ANSI_COLOR_RESET ": izpise spisek podprtih ukazov\n", "help"); 
    printf(ANSI_COLOR_CYAN "%-16s" ANSI_COLOR_RESET ": izpise izhodni status zadnjega (v odspredju) izvedenega ukaza\n", "status"); 
    printf(ANSI_COLOR_CYAN "%-8s" ANSI_COLOR_MAGENTA "%-8s" ANSI_COLOR_RESET ": konca lupino s podanim izhodnim statusom\n", "exit", "<status>"); 
    printf(ANSI_COLOR_CYAN "%-8s" ANSI_COLOR_MAGENTA "%-8s" ANSI_COLOR_RESET ": izpise podane argumente na standardni izhod (brez koncnega skoka v novo vrstico)\n", "print", "<args>"); 
    printf(ANSI_COLOR_CYAN "%-8s" ANSI_COLOR_MAGENTA "%-8s" ANSI_COLOR_RESET ": kot print, le da izpise se skok v novo vrstico\n", "echo", "<args>"); 
    printf(ANSI_COLOR_CYAN "%-16s" ANSI_COLOR_RESET ": izpise pid procesa\n", "pid"); 
    printf(ANSI_COLOR_CYAN "%-16s" ANSI_COLOR_RESET ": izpise ppid starsa\n", "ppid"); 
    printf("\n");

}

void status() {
    exit_status = printf("%d\n", exit_status);
    if (exit_status < 0) perror("status");
    exit_status = 0;
}

void exit_custom_status() {
    exit(atoi(tokens[1]));
}

void print() {
    for (int i = 1; i < token_count; i++) {
        exit_status = printf("%s%s", tokens[i], i == token_count - 1 ? "" : " ");
        if (exit_status < 0) 
            perror("print");
        exit_status = 0;
    }

}

void echo() {
    for (int i = 1; i < token_count; i++) {
        int out = printf("%s%s", tokens[i], i == token_count - 1 ? "\n" : " ");
        if (out < 0) 
            print_error("echo", errno);
    }
    exit_status = 0;
}

void pid() {
    exit_status = printf("%d\n", getpid());
    if (exit_status < 0) perror("pid");
    exit_status = 0;
}

void ppid() {
    exit_status = printf("%d\n", getppid());
    if (exit_status < 0) perror("ppid");
    exit_status = 0;
}

// Zemanjava trenuntnega imenika, ce argumenta ni zamenjamo na /
void dirchange() {
    if (token_count > 1) exit_status = chdir(tokens[1]);
    else exit_status = chdir("/");
    if (exit_status != 0) perror("dirchange");
}

// Izpis trenuntnega imenika
void dirwhere() {
    // TODO: shrani v globalni spremenljivko in izpsi pot primysh>
    int out = printf("%s\n", getwd(NULL));
    if (out < 0) print_error("dirwhere", errno);
    exit_status = 0;
}

void dirmake() {
    int out = mkdir(tokens[1], 0766);
    if (out != 0) print_error("dirmake", errno); 
    else exit_status = 0; 
}

void dirremove() {
    exit_status = 0;
    if (token_count > 1) {
        int out = rmdir(tokens[1]);
        if (out != 0) print_error("dirremove", errno);
    }
}

void dirlist() {
    DIR* mydir;
    if (token_count > 1) 
        mydir = opendir(tokens[1]);
    else mydir = opendir(".");

    struct dirent* myfile;
    char buf[512];

    while ((myfile = readdir(mydir)) != NULL) {
        printf("%s  ", myfile->d_name);
    }
    closedir(mydir);
    printf("\n");
}

void linkhard() {
    int out = link(tokens[1], tokens[2]);
    if (out < 0) print_error("linkhard", errno); 
    else exit_status = 0;
}

void linksoft() {
    int out = symlink(tokens[1], tokens[2]);
    if (out < 0) print_error("linksoft", errno);
    else exit_status = 0;
}

void linkread() {
    char buffer[BUFSIZ];
    if (token_count > 1) {
        int link_string_length = readlink(tokens[1], buffer, sizeof(buffer));
        if (link_string_length < 0) print_error("linkread", errno);
        else {
            buffer[link_string_length] = '\0';
            int out = printf("%s\n", buffer);
            if (out < 0) print_error("linkread", errno);
            else exit_status = 0;
        }
    }
}

// izpise vse trde povzave na datoteko z imenom ime
void linklist() {
    if (token_count == 1) return;
    struct stat file_stat; // get metadata about file
    if (stat(tokens[1], &file_stat) == -1) throw_err(__LINE__, errno);
    ino_t inodeNumber = file_stat.st_ino;

    struct dirent* file;
    DIR* dir;
    char buffer[BUFSIZ]; // get current directory
    if (getcwd(buffer, sizeof(buffer)) == NULL) throw_err(__LINE__, errno);
    if ((dir = opendir(buffer)) == NULL) throw_err(__LINE__, errno);
    int hlink_counter = 0;
    char* hardLinks[MAX_NUMBER_OF_HARDLINKS];

    while ((file = readdir(dir)) != NULL) {
        char* filename = file->d_name;
        if (stat(filename, &file_stat) == -1) {
            // Neveljaven link - nadaljujemo
            continue;
        } 
        if (inodeNumber == file_stat.st_ino) hardLinks[hlink_counter++] = filename;
    }
    for (int i = 0; i < hlink_counter; i++) {
        printf("%s ", hardLinks[i]);
    }
    printf("\n");
    int out = closedir(dir); 
    if (out < 0) print_error("linklist", errno);
    else exit_status = 0;
}

void _unlink() {
    int out = unlink(tokens[1]);
    if (out < 0)  print_error("unlink", errno);
    else exit_status = 0;
}

void _rename() {
    int out = rename(tokens[1], tokens[2]);
    if (out < 0) print_error("rename", errno);
    else exit_status = 0;
}

void cpcat() {
    /* Redirectioin problem -> thats why fork -> */
    if (token_count > 3) { print_error("cpcat", E2BIG); return; }

    int srcDesc = token_count < 2 ? 0 : open(tokens[1], O_RDONLY | O_CREAT, 0644);
    if (srcDesc < 0) throw_err(__LINE__, errno);

    int destDesc = token_count < 3 ? 1 : open(tokens[2], O_WRONLY | O_CREAT, 0644);
    if (destDesc < 0) throw_err(__LINE__, errno);
    
    // prepis
    char buff[BUFSIZ];
    int n_read;
    while ((n_read = read(srcDesc, buff, BUFSIZ)) > 0) {
        if (write(destDesc, buff, n_read) < 0) throw_err(__LINE__, errno);
        n_read = 0;
    }
    if (n_read < 0) throw_err(__LINE__, errno); 

    if (token_count >= 2) {
        if (close(srcDesc) < 0)  throw_err(__LINE__, errno);
    }
    if (token_count >= 3) {
        if (close(destDesc) < 0) throw_err(__LINE__, errno);
    }
}

void pipes() {
    if (token_count < 3) throw_err(__LINE__, EINVAL);
    int fd[2][2];                       // pipe filedescriptors
    char* ptokens[MAX_PIPE_ARGS];       // "<command> args1 .. args2" for each pipe

    // VSTOP V PIPE:    <command> arg1 ... argn |
    if (ptokenize(ptokens, tokens[1]) < 0)          throw_err(__LINE__, errno); 
    if (pipe(fd[0]) < 0)                            throw_err(__LINE__, errno);
    int pid = fork(); 
    if (pid < 0)                                    throw_err(__LINE__, errno);
    if (pid == 0) {
        if (dup2(fd[0][1], 1) < 0)                  throw_err(__LINE__, errno);            
        if (close(fd[0][0]) < 0)                    throw_err(__LINE__, errno); 
        if (close(fd[0][1]) < 0)                    throw_err(__LINE__, errno);
        if (strcmp(ptokens[0], "cpcat") == 0) { pcpcat(ptokens);}
        else { if (execvp(ptokens[0], ptokens) < 0)   throw_err(__LINE__, errno); }
    }

    // SREDINSKI PRIMERI:    | <command> arg1 ... argn |
    for (int pipeLevel = 2; pipeLevel < token_count-1; pipeLevel++) {
        if (ptokenize(ptokens, tokens[pipeLevel]) < 0)  throw_err(__LINE__, errno); 
        if (pipe(fd[(pipeLevel+1)%2]) < 0)                  throw_err(__LINE__, errno);
        int pid = fork();                  if (pid < 0) throw_err(__LINE__, errno);
        if (pid == 0) {
            if (dup2(fd[pipeLevel%2][0], 0) < 0)            throw_err(__LINE__, errno);
            if (dup2(fd[(pipeLevel+1)%2][1], 1) < 0)        throw_err(__LINE__, errno);
            if (close(fd[0][0]) < 0)                        throw_err(__LINE__, errno);
            if (close(fd[0][1]) < 0)                        throw_err(__LINE__, errno);
            if (close(fd[1][0]) < 0)                        throw_err(__LINE__, errno);
            if (close(fd[1][1]) < 0)                        throw_err(__LINE__, errno);
            if (strcmp(ptokens[0], "cpcat") == 0) { pcpcat(ptokens);}
            else { if (execvp(ptokens[0], ptokens) < 0)   throw_err(__LINE__, errno); }
        }
        if (close(fd[pipeLevel%2][0]) < 0)              throw_err(__LINE__, errno);
        if (close(fd[pipeLevel%2][1]) < 0)              throw_err(__LINE__, errno);
    }

    // IZSTOP IZ PIPE:      | <command> args1 args2 
    if (ptokenize(ptokens, tokens[token_count-1]) < 0)          throw_err(__LINE__, errno); 
    pid = fork();                                  if (pid < 0) throw_err(__LINE__, errno);
    if (pid == 0) {
        if (dup2(fd[(token_count-1)%2][0], 0) < 0)            throw_err(__LINE__, errno);            
        if (close(fd[(token_count-1)%2][0]) < 0)              throw_err(__LINE__, errno); 
        if (close(fd[(token_count-1)%2][1]) < 0)              throw_err(__LINE__, errno);
        if (strcmp(ptokens[0], "cpcat") == 0) { pcpcat(ptokens);}
        else { if (execvp(ptokens[0], ptokens) < 0)        throw_err(__LINE__, errno); }
    }
    if (close(fd[(token_count-1)%2][0]) < 0)                throw_err(__LINE__, errno); 
    if (close(fd[(token_count-1)%2][1]) < 0)                throw_err(__LINE__, errno);
    for (int i = 0; i < token_count - 1; i++) wait(NULL);
}

void throw_err(int line, int err_code) {
    char err_buf[128];
    sprintf(err_buf, "%s -> %d", __FILE__, line);
    perror(err_buf);
    exit(err_code);
}

void print_error(char* progName, int err_num) {
    exit_status = err_num;
    errno = err_num;
    perror(progName);
}

void pcpcat(char* ptokens[]) {
    int ptoken_count = 0;
    while (ptokens[ptoken_count] != NULL) {
        ptoken_count += 1;
    }

    if (ptoken_count > 3) { print_error("cpcat", E2BIG); exit(0); }

    int srcDesc = (ptoken_count < 2 || strcmp(ptokens[1], "-") == 0) ? 0 : open(ptokens[1], O_RDONLY);
    if (srcDesc < 0) throw_err(__LINE__, errno);

    int destDesc = (ptoken_count < 3 || strcmp(ptokens[2], "-") == 0) ? 1 : open(ptokens[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (destDesc < 0) throw_err(__LINE__, errno);
    
    // prepis
    char buff[BUFSIZ];
    int n_read;
    while ((n_read = read(srcDesc, buff, BUFSIZ)) > 0) {
        if (write(destDesc, buff, n_read) < 0) throw_err(__LINE__, errno);
        n_read = 0;
    }
    if (n_read < 0) throw_err(__LINE__, errno); 

    if (srcDesc != 0) {
        if (close(srcDesc) < 0)  throw_err(__LINE__, errno);
    }
    if (destDesc != 1) {
        if (close(destDesc) < 0) throw_err(__LINE__, errno);
    }
    exit(0);
}



