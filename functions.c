#include "details.h"
#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>


/*
Set of functions that will be useful throughout
the project.
*/

/* 
There are four functions that the command passes through before finally 
reaching the function that handles that particular command.
colon_separator -> pipe_and_redirect_handler -> parse_command -> fork_handler
and finally execute_command is called.
*/

/*
The functions in this file have been declared / written in the order that they have been called,
for better readability
*/

void colon_separator(char* main_comm);
void parse_command(char *comm);
void fork_handler(char* main_comm, char flags[][50], bool run_in_background, int i);
void execute_command(char* main_comm, char flags[][50], int i, bool bg);
char* get_name(int id, bool forUser);


void colon_separator(char* main_comm) {
    char* single_comm = strtok(main_comm, ";");

    char comms[50][50];
    int k = 0;
    while(single_comm != NULL) {
        if(single_comm != NULL) {
            strcpy(comms[k++], single_comm);   
        }
        single_comm = strtok(NULL, ";");
    }
    for(int i = 0 ; i < k ; i++) {
        parse_command(comms[i]);
    }
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

    bool run_in_background = false;

    char flags[50][50];
    int i = 0;
    while(flag != NULL) {
        flag = strtok(NULL, "   ");
        if(flag != NULL) {
            if(strcmp(flag, "&") == 0) {
                run_in_background = true;
                continue;
            }
            strcpy(flags[i++], flag);
        }
    }
    // flags copied into the flags array

    // if no command is passed return
    if(!main_comm) {
        return;
    }

    fork_handler(main_comm, flags,run_in_background, i);
    // free(comm);
}

void fork_handler(char* main_comm, char flags[][50], bool run_in_background, int i) {

    // not forking if command is cd
    if(strcmp(main_comm, "cd") == 0) {
        change_directory(flags, i);
        return;
    }
    else if(strcmp(main_comm, "pinfo") == 0) {
        proc_flags(flags, i);
        return;
    }
    
    int pid = fork();
    if(pid == 0) {
        execute_command(main_comm, flags, i, run_in_background);
        kill(getpid(), SIGCHLD);
        exit(0);
    }
    else {
        if(!run_in_background){
            int status = 0;
            waitpid(pid, &status, 0);
        }
        fflush(stdout); 
    }
}

void execute_command(char* main_comm, char flags[][50], int i, bool bg) {
    // pwd
    int pid = fork();
    int rv;

    if(pid == 0) {
        if(strcmp(main_comm, "pwd") == 0) {
            print_cwd();
        }
        else if(strcmp(main_comm, "ls") == 0) {
            list_flag_handler(flags, i);
        }
        else if(strcmp(main_comm, "echo") == 0) {
            echo_to_screen(flags, i);
        }
        else {
            int valid_command = foreground_proc(main_comm,flags, i);
            if(valid_command == -1) {
                printf("This is incomprehensible. Please enter something I can understand!\n");
            }
        }
        rv = 0;
        kill(getpid(),  SIGCHLD);
        exit(0);
    }
    else {
        wait(NULL);
        if(bg) {
            printf("Process %s exited\n", main_comm);
            if(rv == 0) {
                printf("normally\n");
            }
            else {
                printf("with error status = %d\n", rv);
            }
            pretty_print();
        }
    }
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