#include "pinfo.h"
#include "main.h"

void pinfo(char p[][50], int i) {
    pid_t pr_id;
    if(i == 0)
        pr_id = getpid();
    else
        pr_id = (pid_t)atoi(p[0]);
    
    char* proc_path = (char *)malloc(sizeof(char) * 100);
    sprintf(proc_path, "/proc/%d/", (int)pr_id);

    DIR* r = opendir(proc_path);
    if(r == NULL) {
        printf("No such process exists. Aborting\n");
        return;
    }

    char* proc_stat = (char *)malloc(sizeof(char) * 100);
    strcpy(proc_stat, proc_path);
    strcat(proc_stat, "/stat");

    int fd_read = open(proc_stat, O_RDONLY);
    if(fd_read < 0){
        perror("Error while reading /proc/<pid>/stat, aborting...");
        return;
    }

    char* proc_file = (char *)malloc(sizeof(char) * 200);
    read(fd_read, proc_file, 200);
    char* file_line = strtok(proc_file, " ");

    printf("pid -- %d\n\n", (int)pr_id);
    int j = 1;
    while(file_line != NULL) {
        if(j == 3) {
            printf("Process Status -- %s\n", file_line);
        }
        if(j == 23) {
            printf("Memory -- %s { Virtual Memory } \n", file_line);
            break;
        }
        j++;
        file_line = strtok(NULL, " ");
    }

    char executable_path[100];
    char* path_to_link = (char*) malloc(sizeof(char) * 200);
    strcpy(path_to_link, proc_path);
    strcat(path_to_link, "exe");
    ssize_t rv = readlink(path_to_link, executable_path, sizeof(executable_path));
    
    if(rv == -1) {
        perror("There was some error in reading the symbolic link. Aborting...");
        return;
    }

    printf("Executable path -- %s\n\n", rel_path(executable_path));
    return;
}