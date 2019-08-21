#include "details.h"
#include <fcntl.h> 
#include <sys/stat.h>

/*
    Set of functions that will be useful throughout
    the project.
*/

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
    // strcpy(name, "ending abruptly testing\n");
    // return name;

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

    // check for empty string
    if(comm[0] == '\0') {
        return;
    }

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
        echo_to_screen(flags, i);
    }
    else if(strcmp(main_comm, "pinfo") == 0) {
        proc_flags(flags, i);
    }
    else {
        printf("Please enter a valid command.\n");
    }
}