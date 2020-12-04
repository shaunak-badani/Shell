#include <sys/types.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h> 

typedef struct p_info {
    char* status;
    pid_t pid;
    char* v_mem;
    char* exec_path;
}pinfo_proc;

pinfo_proc* getpinfo(pid_t pr_id);
void pinfo(char p[][50], int i);