#include "details.h"

#include <fcntl.h>

void add_history(char* comm) {
    char* abs_path_to_history = (char*) malloc(sizeof(char) * 700);
    char* ghar = join_dirs(ENV_HOME, home_dirs);
    strcpy(abs_path_to_history, ghar);
    strcat(abs_path_to_history, "/.history");
    int fd_history = open(abs_path_to_history, O_CREAT | O_APPEND | O_WRONLY, 0700);
    if(fd_history < 0) {
        perror("There was an error while opening history file");
    }
    char hist_comm[700];
    memset(hist_comm, '\0', 700);
    strcpy(hist_comm, comm);
    strcat(hist_comm, "\n");
    int hist_write = write(fd_history, hist_comm, sizeof(hist_comm));
    if(hist_write < 0) {
        perror("There was some error while writing to history");
    }
    free(abs_path_to_history);
    free(ghar);
}

void show_history() {
    char* abs_path_to_history = (char*) malloc(sizeof(char) * 700);
    char* ghar = join_dirs(ENV_HOME, home_dirs);
    strcpy(abs_path_to_history, ghar);
    strcat(abs_path_to_history, "/.history");
    FILE* hist_point = fopen(abs_path_to_history, O_RDONLY);
    tail(hist_point);
}