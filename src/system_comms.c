#include "system_comms.h"
#include "proc.h"
#include "signal_handlers.h"


void handle_command(char* main_comm, char flags[][50], int size_of_flags) {
    // if & exists, need to allocate less space to args
    int k, j;
    int bg = 0;
    for(k = 0 ; k < size_of_flags ; k++) {
        int v = (strcmp(flags[k], "&") == 0);
        bg |= v;
    }

    // Dont bg for vi and vim
    if(strcmp(main_comm, "vi") == 0)
        bg = 0;
    if(strcmp(main_comm, "vim") == 0)
        bg = 0;

    int size_of_args = size_of_flags + 2 - bg;    
    char* args[size_of_args];
    args[0] = main_comm;
    for(j = 0, k = 0 ; k < size_of_args && j < size_of_flags ; j++) {
        int v = (strcmp(flags[j], "&") == 0);
        if(v)
            continue;
        k++;
        args[k] = flags[j];
    }
    k++;
    args[k] = NULL;
    
    int baby = fork();
    if(baby == 0) {
        execvp(main_comm, args);    
    }
    else {
        if(!bg)
            wait(NULL);
        else{
            add_job(main_comm, baby);
        }
    }

    return;
}