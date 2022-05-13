#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/file.h>

int token_count;
char* tokens[50];
char sh_name[100];
char token_buffer[100];
int statp;
int background = 0;
int file_in = 0;
char* file_in_name;
int file_out = 0;
char* file_out_name;
int lastex = 0;
int main_pid;

void process(int s){
    switch(s){
        case 17: //SIGCHLD
            wait(&statp);
            return;
    }
}

void tokenize(char* line){
    token_count = 0;
    int newtoken = 1;
    int multitoken = 0;
    if(line[0] == '#'){
        return;
    }
    int len = strlen(line);
    for(int i = 0; i < len; i++){
        char curr = line[i];
        //printf(":%c: %d %d %d\n", curr, token_count, newtoken, multitoken);
        if(newtoken == 1){
            if(isspace(curr) != 0){
                line[i] = '\0';
                continue;
            }
            if(curr == '"'){
                multitoken = 1;
                tokens[token_count] = &(line[i+1]);
            }
            else {
                multitoken = 0;
                tokens[token_count] = &(line[i]);
            }
            newtoken = 0;

            token_count++;
            continue;
        }
        if((multitoken == 1 && curr == '"') || (multitoken == 0 && isspace(curr))){
            line[i] = '\0';
            newtoken = 1;
        }
    }
}

void name(){
    if(token_count == 1){
        printf("%s\n", sh_name);
        fflush(stdout);
    }
    else {
        strcpy(sh_name, tokens[1]);
    }
    lastex = 0;
}

void help(){
    printf("Help me!\n");
    fflush(stdout);
    lastex = 0;
}

void status(){
    printf("%d\n", lastex);
    fflush(stdout);
    lastex = 0;
}

void print(){
    if(token_count > 1) printf("%s", tokens[1]);
    for(int i = 2; i < token_count; i++){
        printf(" %s", tokens[i]);
    }
    fflush(stdout);
    lastex = 0;
}

void echo(){
    print();
    printf("\n");
    fflush(stdout);
    lastex = 0;
}

void pid(){
    printf("%d\n", main_pid);
    fflush(stdout);
    lastex = 0;
}

void ppid(){
    printf("%d\n", getppid());
    fflush(stdout);
    lastex = 0;
}

void dirchange(){
    int out = 0;
    if(token_count == 1){
        out = chdir("/");
    }
    else {
        out = chdir(tokens[1]);;
    }
    if(out != 0){
        lastex = errno;
        perror("dirchange");
        fflush(stdout);
    }
    else lastex = out;
}

void dirwhere(){
    char cwd[200];
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s\n", cwd);
        lastex = 0;
    }
    else {
        lastex = errno;
        perror("dirwhere");
        fflush(stdout);
    }
}

void dirmake(){
    int out = mkdir(tokens[1], 0755);
    if(out != 0){
        lastex = errno;
        perror("dirmake");
        fflush(stdout);
    }
    else lastex = 0;
}

void dirremove(){
    int out = rmdir(tokens[1]);
    if(out != 0){
        lastex = errno;
        perror("dirremove");
        fflush(stdout);
    }
    else lastex = 0;

}

void dirlist(){
    struct dirent *entry;
    DIR *dir;
    if(token_count == 1){
        char cwd[200];
        if(getcwd(cwd, sizeof(cwd)) == NULL){
            lastex = errno;
            perror("dirlist");
            fflush(stdout);
            return;
        }
        dir = opendir(cwd);
    }
    else {
        dir = opendir(tokens[1]);
    }

    char* dirs[100];
    int counter = 0;
    while ((entry = readdir(dir)) != NULL){
        char* nm = (entry->d_name);
        //if(strcmp(".", nm) == 0 || strcmp("..", nm) == 0) continue;
        dirs[counter++] = nm;
    }
    if(counter > 0) printf("%s", dirs[0]);
    for(int i = 1; i < counter; i++){
        printf("  %s", dirs[i]);
    }
    printf("\n");
    lastex = 0;
}

void linkhard(){
    int out = link(tokens[1], tokens[2]);
    if(out != 0){
        lastex = errno;
        perror("linkhard");
        fflush(stdout);
    }
    else lastex = 0;
}

void linksoft(){
    int out = symlink(tokens[1], tokens[2]);
    if(out != 0){
        lastex = errno;
        perror("linksoft");
        fflush(stdout);
    }
    else lastex = 0;
}

void linkread(){
    char line[100];
    size_t size = 100;
    int out = readlink(tokens[1], line, size);
    if(out == -1){
        lastex = errno;
        perror("linkread");
        fflush(stdout);
    }
    else {
        line[out] = '\0';
        printf("%s\n", line);
        lastex = 0;
    }
}

void linklist(){
    struct dirent *entry;
    DIR *dir;
    char cwd[200];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        lastex = errno;
        perror("linklist");
        fflush(stdout);
        return;
    }

    struct stat file_stat;
    int out = stat(tokens[1], &file_stat);
    if(out != 0){
        lastex = errno;
        perror("linklist");
        fflush(stdout);
        return;
    }
    unsigned long long main_inode = file_stat.st_ino;
    //printf("Main inode: %llu\n", main_inode);

    dir = opendir(cwd);

    char* dirs[100];
    int counter = 0;
    while ((entry = readdir(dir)) != NULL){
        char* nm = (entry->d_name);
        //printf("%s\n", nm);
        //if(strcmp(".", nm) == 0 || strcmp("..", nm) == 0) continue;
        //if(strcmp(nm, tokens[1]) == 0) continue;
        out = stat(nm, &file_stat);
        if(out != 0){
            continue;
        }

        if(main_inode == file_stat.st_ino) dirs[counter++] = nm;
    }
    if(counter > 0) printf("%s", dirs[0]);
    for(int i = 1; i < counter; i++){
        printf(" %s", dirs[i]);
    }
    printf("\n");
    lastex = 0;
}

void unlink1(){
    int out = unlink(tokens[1]);
    if(out != 0){
        lastex = errno;
        perror("unlink");
        fflush(stdout);
    }
    else lastex = 0;
}

void rename1(){
    int out = rename(tokens[1], tokens[2]);
    if(out != 0){
        lastex = errno;
        perror("rename");
        fflush(stdout);
    }
    else lastex = 0;
}

void cpcat(){
    //printf("%s %s %s %d\n", tokens[0], tokens[1], tokens[2], token_count);
    fflush(stdout);
    int inputFD;
    int outputFD;

    if( token_count < 2 ) {
        inputFD = 0;
    }
    else {
        inputFD = open(tokens[1], O_RDONLY, 777);
        if(inputFD < 0){
            lastex = errno;
            perror("cpcat5");
            fflush(stdout);
            return;
        }
    }

    if( token_count < 3 ) {
        outputFD = 1;
    }
    else {
        outputFD = open(tokens[2], O_RDWR | O_CREAT, 777);
        if(outputFD < 0){
            lastex = errno;
            perror("cpcat6");
            fflush(stdout);
            return;
        }
    }

    char ch = ' ';
    int ret;
    while(ch != EOF){
        ret = read(inputFD, &ch, 1);
        //printf("\n:%d:\n", ch);
        if(ret == 0) break;
        if(ret < 0){
            lastex = errno;
            perror("cpcat1");
            fflush(stdout);
            break;
        }
        ret = write(outputFD, &ch, 1);
        if(ret < 0){
//            lastex = errno;
//            perror("cpcat2");
//            fflush(stdout);
            break;
        }
    }
    //ret = close(inputFD);
    if(inputFD != 0) ret = close(inputFD);
//    if(ret < 0){
//        lastex = errno;
//        perror("cpcat3");
//        fflush(stdout);
//        return;
//    }
    //ret = close(outputFD);
    if(outputFD != 1) ret = close(outputFD);
//    if(ret < 0){
//        lastex = errno;
//        perror("cpcat4");
//        fflush(stdout);
//        return;
//    }
}

int main(int argc, char *argv[]){
	signal(SIGCHLD, process);

	main_pid = getpid();

	strcpy(sh_name, "mysh");

	while(1){
	    char* line = NULL;
	    size_t size;
	    if(isatty(1)) printf("%s>", sh_name);
	    int out = getline(&line, &size, stdin);
	    if(out == -1){
	        return 0;
	    }
	    tokenize(line);
	    if(token_count == 0){
	        continue;
	    }


	    if(strcmp(tokens[token_count-1], "&") == 0){
	        background = 1;
	        token_count--;
	    }
	    else {
	        background = 0;
	    }

	    if(tokens[token_count-1][0] == '>'){
	        file_out = 1;
	        file_out_name = &(tokens[token_count-1][1]);
	        token_count--;
	        //TODO open file!
	    }
	    else {
	        file_out = 0;
	    }

	    if(tokens[token_count-1][0] == '<'){
	        file_in = 1;
	        file_in_name = &(tokens[token_count-1][1]);
	        token_count--;
	        //TODO open file!
	    }
	    else {
	        file_in = 0;
	    }

	    //printf("\nTokens:\n");
	    for(int i = 0; i < token_count; i++){
	        //printf(":%s:\n", tokens[i]);
	    }
	    //printf("Modifiers: IN_REDIR=%d (%s) OUT_REDIR=%d (%s) BKGRND=%d\n\n", file_in, file_in_name, file_out, file_out_name, background);

	    int cpid1 = -1;
	    if(background == 1){
	        cpid1 = fork();
	        if(cpid1 != 0) continue;
	    }

	    FILE* old_stdout = stdout;
	    if(file_out == 1){
	        stdout = fopen(file_out_name, "w");
	    }

	    FILE* old_stdin = stdin;
	    if(file_in == 1){
	        stdin = fopen(file_in_name, "r");
	    }

	    if(strcmp(tokens[0], "name") == 0)           name();
	    else if(strcmp(tokens[0], "help") == 0)      help();
	    else if(strcmp(tokens[0], "status") == 0)    status();
	    else if(strcmp(tokens[0], "exit") == 0)      exit(atoi(tokens[1]));
	    else if(strcmp(tokens[0], "print") == 0)     print();
	    else if(strcmp(tokens[0], "echo") == 0)      echo();
	    else if(strcmp(tokens[0], "pid") == 0)       pid();
	    else if(strcmp(tokens[0], "ppid") == 0)      ppid();

	    else if(strcmp(tokens[0], "dirchange") == 0) dirchange();
	    else if(strcmp(tokens[0], "dirwhere") == 0)  dirwhere();
	    else if(strcmp(tokens[0], "dirmake") == 0)   dirmake();
	    else if(strcmp(tokens[0], "dirremove") == 0) dirremove();
	    else if(strcmp(tokens[0], "dirlist") == 0)   dirlist();

	    else if(strcmp(tokens[0], "linkhard") == 0)  linkhard();
	    else if(strcmp(tokens[0], "linksoft") == 0)  linksoft();
	    else if(strcmp(tokens[0], "linkread") == 0)  linkread();
	    else if(strcmp(tokens[0], "linklist") == 0)  linklist();
	    else if(strcmp(tokens[0], "unlink") == 0)    unlink1();
	    else if(strcmp(tokens[0], "rename") == 0)    rename1();
	    else if(strcmp(tokens[0], "cpcat") == 0)     cpcat();
	    else {
	        tokens[token_count] = NULL;
            int fileout;
            if(file_out == 1){
                fileout = open(file_out_name, O_WRONLY|O_CREAT, 0777);
            }
            int filein;
            if(file_in == 1){
                filein = open(file_in_name, O_RDONLY);
            }
	        int cpid = fork();
	        if(cpid == 0){
	            if(file_out == 1){
                    dup2(fileout, 1);
                }
	            if(file_in == 1){
                    dup2(filein, 0);
                }
	            return execvp(tokens[0], tokens);
	        }
	        else {
	            if(file_out == 1){
	                close(fileout);
                }
	            if(file_in == 1){
	                close(filein);
                }
	            if(cpid1 != 0) wait(&lastex);
	        }
	    }
	    if(cpid1 == 0) {
	        exit(lastex);
	        return lastex;
	    }
	    if(file_out == 1){
	        stdout = old_stdout;
	    }
	    if(file_in == 1){
	        stdin = old_stdin;
	    }
	}
}



