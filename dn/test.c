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

