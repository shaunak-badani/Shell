#include "main.h"
#include <fcntl.h> 
#include <sys/stat.h>
#include "utils.h"
#include "builtin.h"
#include "pinfo.h"
#include "system_comms.h"
#include "proc.h"

/*
    Set of functions that will be useful throughout
    the project.
*/


void remove_newline(char* command, int no_of_chars) {
    command[no_of_chars - 1] = '\0';
}

// fetches name corresponding to user / group
char* get_name(int id, bool forUser) {
    // if forUser is set, will fetch user name
    // else will fetch group name
    char* name = (char*) malloc(sizeof(char) * 100);
    struct stat st;

    char* file_name = (char*) malloc(sizeof(char) * 200);
    
    if(forUser) {
        strcpy(file_name, "/etc/passwd");
    }
    else {
        strcpy(file_name, "/etc/group");
    }

    int fd_etc_passwd = open(file_name, O_RDONLY);
    if(fd_etc_passwd < 0) {
        perror("There was an error in opening /etc/passwd\n");
        strcpy(name, "?????");
        return name;
    }
    
    // reading file /etc/password to get username
    stat(file_name, &st);
    char *passwd_file = (char *)malloc(sizeof(char) * st.st_size);
    read(fd_etc_passwd, passwd_file, st.st_size);
    char* file_line = strtok(passwd_file, "\n");
    char file_lines[500][100];

    int j = 0;
    while(file_line != NULL) {
        // printf("%s\n", file_lines[j]);
        if( file_line != NULL ) {
            strcpy(file_lines[j++], file_line);
        }
        file_line = strtok(NULL, "\n");
    }

    char* user = (char*)malloc(sizeof(char) * 1000);
    char* temp = (char*)malloc(sizeof(char) * 1000);
    char* uid = (char*)malloc(sizeof(char) * 100);
    int len = sizeof(file_lines) / sizeof(char);
    j--;
    for( ; j >= 0 ; j--) {
        user = strtok(file_lines[j],":");
        temp = strtok(NULL,":");
        uid = strtok(NULL, ":");
        if(atoi(uid) == (int)id) {
            strcpy(name, user);
            return name;
        }
    }
    return user;
}

/* Apart from parsing the command, this function also acts as middleware for 
   every command that is input into the shell
*/
void parse_command(char *comm){
    if(comm[0] == '\0')
        return;

    char* main_comm = strtok(comm, " ");
    char *flag = main_comm;

    char flags[50][50];
    int i = 0;
    while(flag != NULL) {
        flag = strtok(NULL, " ");
        if(flag != NULL) {
            strcpy(flags[i++], flag);
        }
    }
    // flags copied into the flags array
    
    // pwd
    if(strcmp(main_comm, "pwd") == 0) {
        print_cwd();
    }
    else if(strcmp(main_comm, "cd") == 0) {
        change_directory(flags, i);
    }
    else if(strcmp(main_comm, "ls") == 0) {
        list_flag_handler(flags, i);
    }
    else if(strcmp(main_comm, "echo") == 0) {
        echo(flags, i);
    }
    else if(strcmp(main_comm, "pinfo") == 0) {
        pinfo(flags, i);
    }
    else if(strcmp(main_comm, "jobs") == 0) {
        jobs(flags, i);
    }
    else if(strcmp(main_comm, "kjob") == 0) {
        kjob(flags, i);
    }
    else if(strcmp(main_comm, "overkill") == 0) {
        overkill(flags, i);
    }
    else {
        handle_command(main_comm, flags, i);
    }
}

// Relative paths : 
// returns the path relative to the home directory
char* display_directory_arr(char dir[][50], int size_of_dir) {
    char* relative_path = (char*) malloc(sizeof(char) * 1000);
    strcpy(relative_path, "~");
    int loop_length = size_of_dir <= home_dirs ? size_of_dir : home_dirs;
    // if equal, loop_length will be cwd size
    int i;
    for(i = 0; i < loop_length ; i++) {
        if(strcmp(dir[i], ENV_HOME[i]) != 0) {
            break;
        }
    }
    if(size_of_dir >= home_dirs) {
        if(i == loop_length) {
            for( ; i < size_of_dir ; i++) {
                strcat(relative_path, "/");
                strcat(relative_path, dir[i]);
            }
            return relative_path;
        }
        else {
            return join_dirs(dir, size_of_dir);
        }
    }
    else {
        return join_dirs(dir, size_of_dir);
    }
}

// returns any path relative to the home path
char* rel_path(char* path_string) {
    char* folder_name = strtok(path_string, "/");
    char folders_arr[30][50]; 
    int i = 0;
    while(folder_name != NULL){
        if(folder_name != NULL) {
            strcpy(folders_arr[i++], folder_name);
        }
        folder_name = strtok(NULL, "/");
    }
    return display_directory_arr(folders_arr, i);
}