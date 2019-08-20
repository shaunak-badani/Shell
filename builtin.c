#include "details.h"

/* Just some function declarations */
char* join_dirs(char dir_array[][50], int len_of_array);
char* display_directory();

// function to handle printing of current working directory == pwd
void print_cwd() {
    char current_dir[PATH_MAX];
    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        printf("%s\n", current_dir);
    } else {
        perror("getcwd() error");
    }
}

// function to handle echo
void echo_to_screen(char flags[][50], int size_of_flags) {
    for(int i = 0 ; i < size_of_flags ; i++) {
        printf("%s ", flags[i]);
    }
    printf("\n");
}

void change_directory(char flags[][50], int size_of_flags) {
    char current_dir[PATH_MAX]; 
    getcwd(current_dir, sizeof(current_dir));
    if(size_of_flags == 1) {
        if(strcmp(flags[0], ".") == 0) {
            chdir(".");
            return;
        }
        else if(strcmp(flags[0], "..") == 0) {
            strcpy(CWD[size_of_folder_dirs - 1], "\0");
            size_of_folder_dirs--;
            char* current_dir = join_dirs(CWD, size_of_folder_dirs);
            chdir("..");
        }
        else if(strcmp(flags[0], "~") == 0) {
            for(int j = 0; j < home_dirs ; j++) {
                strcpy(CWD[j], ENV_HOME[j]);
            }
            size_of_folder_dirs = home_dirs;
            chdir(join_dirs(CWD, home_dirs));
        }
        else {
            chdir(flags[0]);
            char* folder_name = strtok(flags[0], "/");
            while(folder_name != NULL){
                if(strcmp(folder_name, "..") == 0) {
                    strcpy(CWD[size_of_folder_dirs - 1], "\0");
                    size_of_folder_dirs--;
                }
                else {
                    strcpy(CWD[size_of_folder_dirs], folder_name);
                    size_of_folder_dirs++;
                }
                folder_name = strtok(NULL, "/");
            }
            char* current_dir = join_dirs(CWD, size_of_folder_dirs);
        }
    }
    else {
        printf("Please note that your folder name must escape spaces\n");
        printf("Usage : cd <folder-to-go-to> \n");
    }
}

/* helper functions for directory operations */

// joins the folder names in dir array passed to it and returns a string
char* join_dirs(char dir_array[][50], int len_of_array) {
    char *current_dir = (char *)malloc(sizeof(char) * 500);
    for(int i = 0 ; i < len_of_array ; i++) {
        strcat(current_dir, "/");
        strcat(current_dir, dir_array[i]);
    }
    return current_dir;
}


// returns the path relative to the home directory
char* display_directory() {
    char* relative_path = (char*) malloc(sizeof(char) * 1000);
    strcpy(relative_path, "~");
    int loop_length = size_of_folder_dirs <= home_dirs ? size_of_folder_dirs : home_dirs;
    // if equal, loop_length will be cwd size
    int i;
    for(i = 0; i < loop_length ; i++) {
        if(strcmp(CWD[i], ENV_HOME[i]) != 0) {
            break;
        }
    }
    if(size_of_folder_dirs >= home_dirs) {
        if(i == loop_length) {
            for( ; i < size_of_folder_dirs ; i++) {
                strcat(relative_path, "/");
                strcat(relative_path, CWD[i]);
            }
            return relative_path;
        }
        else {
            return join_dirs(CWD, size_of_folder_dirs);
        }
    }
    else {
        return join_dirs(CWD, size_of_folder_dirs);
    }
}