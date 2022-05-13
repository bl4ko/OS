#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/resource.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXPATHLEN 1024
#define MAXNUMBEROFPROCESSES 5000

typedef struct Process {
    char* PID;
    char* PPID;
    char* State;
    char* Name;
    char* Threads;
    int Files;
    struct Process* next;
} process;

typedef struct ProcessList {
    process* first;
    process* last;
} processList;

void systemInfo(char* path);
void firstSwapPartition(char* path);
void countModules(char* path);
char* itoa(int val, int base);
void printProcessFolders(char* path);
int isNumber(char* string);
int isDirectory(const char* path);
void appendSorted(int* arr, int num, int arr_len); 
processList* createProcessList(char* path, char* orderBy);
void processListAdd(char* path, processList* list, char* orderBy);
int strcicmp(char const *a, char const *b);
void findChildrenPidsRec(processList* list, char* PPID, char** array, int* arr_len); 
int hasList(char** arr, int* arrLen, char* value); 
void forktree();
void recForkTree(int indeksOtroka, int* arr, int* komulativa); 


int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("Premalo argumentov: ./Naloga2 akcija parametri\n");
        exit(1);
    }

    char* akcija = argv[1];

    if (strcmp(akcija, "sys") == 0) {
        if (argc < 3) {
            fprintf(stderr, "No file path was given: sys <file_path>\nExiting!\n");
            exit(1);
        }
        systemInfo(argv[2]);
    } 
    else if (strcmp(akcija, "sysext") == 0) {
        if (argc < 3) {
            fprintf(stderr, "No file path was given: sysext <file_path>\nExiting!\n");
            exit(1);
        }
        systemInfo(argv[2]);
        firstSwapPartition(argv[2]);
        countModules(argv[2]);
        exit(0);
    } 
    else if (strcmp(akcija, "me") == 0) { 
        // TODO
        char buf[1024];
        char tmp[30];
        pid_t PID = getpid();
        printf("Uid: %d\n", (int) getuid()); // UID
        printf("EUid: %d\n" , (int) geteuid()); // Effective UID
        printf("Gid: %d\n", (int) getgid());
        printf("EGid: %d\n", (int) getegid());
        printf("Cwd: %s\n", getcwd(buf, 1024));
        printf("Priority: %d\n", getpriority(PRIO_PROCESS, PID)); 
        memset(buf, 0, 1024); // empty buffer
        strcat(buf, "/proc/");
        strcat(buf, itoa((int)PID, 10));
        strcat(buf, "/");
        printf("Process proc path: %s\n", buf);
        printf("Process proc access: %s\n", access(buf, R_OK) == 0 ? "yes" : "no");
        // System information
        struct utsname unameBuffer;
        uname(&unameBuffer);
        printf("OS name: %s\n", unameBuffer.sysname);
        printf("OS release: %s\n", unameBuffer.release);
        printf("OS version: %s\n", unameBuffer.version);
        printf("Machine: %s\n", unameBuffer.machine);
        printf("Node name: %s\n", unameBuffer.nodename);
        // -- Time zone -----------------------------------------------------------
        struct timeval tv; 
        struct timezone tz;
        gettimeofday(&tv, &tz);
        printf("Timezone: %d\n",  tz.tz_dsttime);
        // -- CPU time  ------------------------------------------------------------
        struct rlimit rlim;
        getrlimit(RLIMIT_CPU, &rlim);
        printf("CPU limit: %llu\n", rlim.rlim_max);
        exit(0);
    }
    else if (strcmp(akcija, "pids") == 0) {
        printProcessFolders(argv[2]); 
        exit(0);
    } 
    else if (strcmp(akcija, "names") == 0) { 
        processList* list = createProcessList(argv[2], "Name");
        process* sprehajalec = list->first;
        while (sprehajalec != NULL) {
            printf("%s %s\n", sprehajalec->PID, sprehajalec->Name);
            process* prejsni = sprehajalec;
            sprehajalec = sprehajalec -> next;
            free(prejsni);
        }
        free(list);
        exit(0);
    }
    else if (strcmp(akcija, "ps") == 0 && argc == 3) {
        processList* list = createProcessList(argv[2], "PID");
        process* sprehajalec = list->first;
        printf("%5s %5s %6s %s\n",  "PID", "PPID", "STANJE", "IME");
        int prednik;
        while (sprehajalec != NULL) {
            printf("%5s %5s %6s %s\n", sprehajalec->PID, sprehajalec->PPID, sprehajalec->State, sprehajalec->Name);
            process* prejsni = sprehajalec;
            sprehajalec = sprehajalec -> next;
            free(prejsni);
        }
        free(list);
        exit(0);
    } 
    else if (strcmp(akcija, "ps") == 0 && argc == 4) {
        processList* list = createProcessList(argv[2], "PID");
        char* prednikPPID = argv[3];
        char** potomciPIDS = calloc(100, sizeof(char*)); potomciPIDS[0] = prednikPPID;
        int* stPotomcev = malloc(sizeof(int)); *stPotomcev = 1;
        findChildrenPidsRec(list, prednikPPID, potomciPIDS, stPotomcev);
        process* sprehajalec = list->first;
        printf("%5s %5s %6s %s\n",  "PID", "PPID", "STANJE", "IME");
        int prednik;
        while (sprehajalec != NULL) {
            if (hasList(potomciPIDS, stPotomcev, sprehajalec->PID)) {
                printf("%5s %5s %6s %s\n", sprehajalec->PID, sprehajalec->PPID, sprehajalec->State, sprehajalec->Name);
            }
            process* prejsni = sprehajalec;
            sprehajalec = sprehajalec -> next;
            free(prejsni);
        }
        free(list);
        exit(0);
    }
    else if (strcmp(akcija, "psext") == 0 && argc == 3) { 
        printf("%5s %5s %6s %6s %6s %s\n", "PID", "PPID", "STANJE", "#NITI", "#DAT.", "IME");
        processList* list = createProcessList(argv[2], "PID");
        process* sprehajalec = list->first;
        while (sprehajalec != NULL) {
            printf("%5s %5s %6s %6s %6d %s\n", sprehajalec->PID, sprehajalec->PPID, sprehajalec->State, sprehajalec->Threads, sprehajalec->Files, sprehajalec->Name);
            process* prejsni = sprehajalec;
            sprehajalec = sprehajalec -> next;
            free(prejsni);
        }
        exit(0);
    }
    else if (strcmp(akcija, "psext") == 0 && argc == 4) { 
        char* PPID = argv[3]; 
        processList* list = createProcessList(argv[2], "PID");
        char** potomciPids = calloc(500, sizeof(char*)); potomciPids[0] = PPID;
        int* potomci_len = malloc(sizeof(int)); *potomci_len = 1;
        findChildrenPidsRec(list, PPID, potomciPids, potomci_len);
        printf("%5s %5s %6s %6s %6s %s\n", "PID", "PPID", "STANJE", "#NITI", "#DAT.", "IME");
        process* sprehajalec = list->first;
        while (sprehajalec != NULL) {
            if (hasList(potomciPids, potomci_len, sprehajalec->PID)) {
                printf("%5s %5s %6s %6s %6d %s\n", sprehajalec->PID, sprehajalec->PPID, sprehajalec->State, sprehajalec->Threads, sprehajalec->Files, sprehajalec->Name);
            }
            process* prejsni = sprehajalec;
            sprehajalec = sprehajalec -> next;
            free(prejsni);
        }
        free(potomciPids);
        exit(0);
    }
    else if (strcmp(akcija, "forktree") == 0) { 
        forktree();
        exit(0);
    }
    else {
        fprintf(stderr, "Napacna akcija!\n");
    }
}

void forktree() {
    int* arr = calloc(3000, sizeof(int));
    int* komulativa = calloc(3000, sizeof(int));
    int sum = 0;
    int arr_len = 0;
    while (scanf("%d", &arr[arr_len]) == 1) {
        komulativa[arr_len] = sum;
        sum += arr[arr_len];
        arr_len += 1;
    }    
    komulativa[arr_len] = sum;
    recForkTree(0, arr, komulativa);
    sleep(1);
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        char ppid[10];
        sprintf(ppid, "%d", getppid());
        execl("/usr/bin/pstree", "pstree", "-c", ppid, (char *) 0);
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
    free(komulativa); 
    free(arr);
}

void recForkTree(int indeksOtroka, int* arr, int* komulativa) {
    int stOtrok = arr[indeksOtroka];
    for (int i = 1; i <= stOtrok; i++) {
        int childPid = fork();
        if (childPid == 0) {
            recForkTree(komulativa[indeksOtroka] + i, arr, komulativa);
            sleep(2);
            exit(0);
        }
    }
}

int hasList(char** arr, int* arrLen, char* value) {
    for (int i = 0; i < *arrLen; i++) {
        if (strcmp(arr[i], value) == 0) 
            return 1;
    }
    return 0;
}

processList* createProcessList(char* path, char* orderBy) {
    DIR *srcdir = opendir(path);
    processList* list = calloc(1, sizeof(processList));
    struct dirent *file;
    if (!srcdir) {
        if (errno == ENOENT) {
            perror("Directory doesn't exist");
        } else {
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }
    while ((file = readdir(srcdir)) != NULL) {
        if (isNumber(file->d_name)) {
            char* processPath = malloc(sizeof(char) * 40);
            strcat(processPath, path);
            strcat(processPath, "/");
            strcat(processPath, file->d_name);
            processListAdd(processPath, list, orderBy);
            free(processPath);
        }
    }
    closedir(srcdir);
    return list;
}

void processListAdd(char* path, processList* list, char* orderBy) {
    char* pstatus_file_path = calloc(40, sizeof(char));
    strcat(pstatus_file_path, path);
    strcat(pstatus_file_path, "/status");
    // printf("Adding process with path: %s, method: %s\n", path, orderBy);
    FILE* pstatus_file = fopen(pstatus_file_path, "r");
    free(pstatus_file_path);
    if (pstatus_file == NULL) {
        fprintf(stderr, "Error opening file with path: %s\n", pstatus_file_path);
        exit(1);
    }
    char buffer[1024];
    int line = 0;
    process* newProcess = malloc(sizeof(process));
    while (fgets(buffer, 1024, pstatus_file)) {
        char* pAttribute = calloc(20, sizeof(char));;
        char* pValue = calloc(20, sizeof(char));
        sscanf(buffer, "%[^:]:%s", pAttribute, pValue);
        // printf("pAttribute: %s\n", pAttribute);
        if (strcmp(pAttribute, "Name") == 0) {  
            newProcess -> Name = pValue;
        }
        else if (strcmp(pAttribute, "State") == 0) {
            newProcess -> State = pValue;
        }
        else if (strcmp(pAttribute, "Pid") == 0) {
            newProcess -> PID = pValue;
        }
        else if (strcmp(pAttribute, "PPid") == 0) {
            newProcess -> PPID = pValue;
        } else if (strcmp(pAttribute, "Threads") == 0) {
            newProcess -> Threads = pValue;
            fclose(pstatus_file);
            break;
        }
        line++;
    }
    // Preverimo se stevilo datotek ki jih proces uporablja
    int numOfProcesses = 0;
    char* fd_path = calloc(40, sizeof(char));
    strcat(fd_path, path);
    strcat(fd_path, "/fd");
    DIR *srcdir = opendir(fd_path);
    struct dirent *file;
    if (!srcdir) {
        if (errno == ENOENT) {
            perror("Directory doesn't exist");
        } else {
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }
    while ((file = readdir(srcdir)) != NULL) {
        if (isNumber(file->d_name)) {
            numOfProcesses++;
        }
    }
    closedir(srcdir);
    newProcess->Files = numOfProcesses;
    // printf("newProcess created: Name: %s, State: %s, Pid: %s, PPid: %s, Threads: %s, Files: %s\n", newProcess->Name, newProcess->State, newProcess->PID, newProcess->PPID, newProcess->Threads, newProcess->Files);
    process* prejsni = NULL; 
    process* trenutni = list->first;
    while (trenutni != NULL) {
        int sortByName =  strcicmp(newProcess->Name,trenutni->Name);
        int sortByPID = atoi(newProcess->PID) - atoi(trenutni->PID);
        if (strcmp(orderBy, "PID") == 0) {
            if (sortByPID < 0) 
                break;
        }
        else if (strcmp(orderBy, "Name") == 0) {
            if (sortByName < 0 || sortByName == 0 && sortByPID < 0) 
                break;
        }
        prejsni = trenutni;
        trenutni = trenutni -> next;
    }
    newProcess -> next = trenutni;
    if (list->first == NULL || trenutni != NULL && strcmp(trenutni->PID, list->first->PID) == 0) {
        list->first = newProcess;
    }
    if (prejsni != NULL) {
        prejsni->next = newProcess;
    }
}

void findChildrenPidsRec(processList* list, char* PPID, char** array, int* arr_len) {
    char** novi = malloc(sizeof(char*) * 20);
    int stNovih = 0;
    process* sprehajalec = list->first;
    while (sprehajalec != NULL) {
        if (strcmp(sprehajalec->PPID, PPID) == 0) {
            novi[stNovih] = sprehajalec->PID;
            stNovih++;
            array[*arr_len] = sprehajalec->PID;
            *arr_len+=1;
        }
        sprehajalec = sprehajalec -> next;
    }
    for (int i = 0; i < stNovih; i++) {
        findChildrenPidsRec(list, novi[i], array, arr_len);
    }
};

char* itoa(int val, int base){
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

void systemInfo(char* path) {
    char* version_file_path = calloc(MAXPATHLEN, sizeof(char));
    strcat(version_file_path, path);
    strcat(version_file_path, "/version");
    FILE* file = fopen(version_file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file with path: %s\n", version_file_path);
        exit(1);
    };
    char tmp[1024];
    char* kernel_version = calloc(1024, sizeof(char));
    char* gcc_version = calloc(1024, sizeof(char));
    int i = 0;
    while (fscanf(file, " %1024s", tmp) == 1) {
        if (i == 2) {
            strcpy(kernel_version, tmp);
        } else if (i == 6) {
            strcpy(gcc_version, tmp);
            break;
        }
        i++;
    }
    fclose(file);
    printf("Linux: %s\ngcc: %s\n", kernel_version, gcc_version);
    free(version_file_path);
    free(kernel_version);
    free(gcc_version);
}

void firstSwapPartition(char* path) {
    char* swaps_file_path = calloc(MAXPATHLEN, sizeof(char));
    strcat(swaps_file_path, path);
    strcat(swaps_file_path, "/swaps"); 
    FILE* file = fopen(swaps_file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file with path: %s\n", swaps_file_path);
        exit(1);
    };
    char buffer[1024];
    int i = 0;
    while (fgets(buffer, 1024, file)) {
        if (i == 1) {
            char* token = strtok(buffer, " ");
            printf("Swap: %s\n", token);
            break;
        }
        i++;
    }
    fclose(file);
    free(swaps_file_path);
}

int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

void countModules(char* path) {
    char* modules_file_path = calloc(MAXPATHLEN, sizeof(char));
    strcat(modules_file_path, path);
    strcat(modules_file_path, "/modules"); 
    FILE* file = fopen(modules_file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file with path: %s\n", modules_file_path);
        exit(1);
    };
    char buffer[1024];
    int num_of_lines = 0;
    while (fgets(buffer, 1024, file)) {
        num_of_lines++;
    }
    printf("Modules: %d\n", num_of_lines);
    fclose(file);
    free(modules_file_path);
    exit(num_of_lines);
}

void printProcessFolders(char* path) {
    int* processes_ids = calloc(MAXNUMBEROFPROCESSES, sizeof(int));
    int numOfProcesses = 0;
    DIR *srcdir = opendir(path);
    struct dirent *file;
    if (!srcdir) {
        if (errno == ENOENT) {
            perror("Directory doesn't exist");
        } else {
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }
    while ((file = readdir(srcdir)) != NULL) {
        char* fullPath = malloc(sizeof(char) * 40);
        // strcpy(fullPath, "./");
        // strcat(fullPath, path);
        // strcat(fullPath, "/");
        // strcat(fullPath, file->d_name);
        // if (isDirectory(fullPath)) {
            if (isNumber(file->d_name)) {
                appendSorted(processes_ids, atoi(file->d_name), numOfProcesses);
                numOfProcesses++;
            }
        // }
        free(fullPath);
    }
    closedir(srcdir);
    for (int i = 0; i < numOfProcesses; i++) 
        printf("%d\n", processes_ids[i]);
    free(processes_ids);
}

void appendSorted(int* arr, int num, int arr_len) {
   for (int i = 0; i < arr_len; i++) {
       if (arr[i] > num) {
           for (int j = arr_len; j > i; j--) {
               arr[j] = arr[j-1];
           }
           arr[i] = num;
           return;
       }
   } 
    arr[arr_len] = num;
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int isNumber(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (!isdigit(string[i])) 
            return 0;
    }
    return 1;
}

// char* osName(char* path) {
//     char* version_file_path = calloc(MAXPATHLEN, sizeof(char));
//     strcat(version_file_path, path);
//     strcat(version_file_path, "/version"); 
//     FILE* file = fopen(version_file_path, "r");
//     if (file == NULL) {
//         fprintf(stderr, "Error opening file with path: %s\n", version_file_path);
//         exit(1);
//     };
//     char buffer[1024];
//     char* osName[30];
//     while (fgets(buffer, 1024, file)) {
//         char* osName = strtok(buffer, " ");
//         break;
//     }
//     fclose(file);
//     free(version_file_path);
//     return osName;
// }
