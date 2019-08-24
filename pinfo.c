#include "details.h"
#include <dirent.h>
#include<fcntl.h> 

// prints the details of the process for the process id passed
void process_desc(int process_id) {
    char* proc_path = (char *)malloc(sizeof(char) * 100);
    sprintf(proc_path, "/proc/%d", process_id);

    if(opendir(proc_path) == NULL) {
        printf("No such process exists. Aborting \n");
        return;
    }

    char* proc_stat = (char *)malloc(sizeof(char) * 100);
    strcpy(proc_stat, proc_path);
    strcat(proc_stat, "/stat");

    int fd_read = open(proc_stat, O_RDONLY);
    if(fd_read < 0) {
        perror("Error while reading /proc/<pid>/stat");
        return;
    }
    // parsing /proc/pid/stat
    char *proc_file = (char *)malloc(sizeof(char) * 200);
    read(fd_read, proc_file, 200);
    char* file_line = strtok(proc_file, " ");

    // process exists
    printf("pid -- %d\n\n", process_id);
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
    strcat(path_to_link, "/exe");
    ssize_t rv = readlink(path_to_link, executable_path, sizeof(executable_path));
    if(rv == -1) {
        perror("There was some error in reading the symbolic link");
        return;
    }
    printf("Executable path -- %s\n", rel_path(executable_path));
}

void proc_flags(char flags[][50], int size_of_flags) {
    if(size_of_flags == 0) {
        process_desc((int)getpid());
    }
    else if(size_of_flags == 1) {
        int process_id;
        process_id = atoi(flags[0]);
        printf("process id passed : %d\n", process_id);
        process_desc(process_id);
    }
}