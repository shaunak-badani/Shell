#include "details.h"
#include <sys/wait.h>

int foreground_proc(char* main_comm, char flags[][50], int size_of_flags) {
    char* args[50];

    args[0] = (char*) malloc(sizeof(char) * 100);
    strcpy(args[0], main_comm);
    int i;
    
    for(i = 1 ; i < size_of_flags + 1; i++) {
        args[i] = (char*)malloc(sizeof(char) * 20);
        memset(args[i], '\0', 20*sizeof(char));
        strcpy(args[i], flags[i - 1]);
    }
    args[i] = NULL;
    return execvp(args[0], args);
}