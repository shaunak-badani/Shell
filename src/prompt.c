#include "main.h"
#include <sys/stat.h>
#include <fcntl.h> 
#include "prompt.h"
// contains all the macros

struct stat st;

/*
This function sets the user id, username, systemname, and current directory.
*/
void initialize_details(){
    // get user id from syscall getuid
    userId = getuid();
    int fd_etc_passwd = open("/etc/passwd", O_RDONLY);
    
    // reading file /etc/password to get username
    stat("/etc/passwd", &st);
    char *passwd_file = (char *)malloc(sizeof(char) * st.st_size);
    read(fd_etc_passwd, passwd_file, st.st_size);
    char* file_line = strtok(passwd_file, "\n");
    char file_lines[500][100];

    int j = 0;
    while(file_line != NULL) {
        strcpy(file_lines[j++], file_line);
        file_line = strtok(NULL, "\n");
    }

    char* user = (char*)malloc(sizeof(char) * 1000);
    char* uid = (char*)malloc(sizeof(char) * 100);
    j--;
    for( ; j >= 0 ; j--) {
        user = strtok(file_lines[j],":");
        strtok(NULL,":");
        uid = strtok(NULL, ":");
        if(atoi(uid) == userId) {
            strcpy(username, user);
            break;
        }
    }
    // finished reading username;

    // reading system name from /etc/hostname
    int fd_hostname = open("/etc/hostname", O_RDONLY);
    stat("/etc/hostname", &st);
    char *hostname_file = (char *)malloc(sizeof(char) * st.st_size);
    read(fd_hostname, hostname_file, st.st_size);

    char* sysname = strtok(hostname_file, "\n");
    strcpy(system_name, sysname);

    //setting home as current directory
    char current_dir[PATH_MAX];
    getcwd(current_dir, sizeof(current_dir));
    char* folder_name = strtok(current_dir, "/");
    int i = 0;
    while(folder_name != NULL){
        if(folder_name != NULL) {
            strcpy(CWD[i], folder_name);
            strcpy(ENV_HOME[i], folder_name);
        }
        i++;
        folder_name = strtok(NULL, "/");
    }
    size_of_folder_dirs = i;
    home_dirs = i;
}


// Function to print the prompt in the format required
void pretty_print() {

    // username in green
    printf("\033[0;32m");
    printf("%s", username);

    // @ in bold yellow
    printf("\033[01;33m");
    printf("@");

    // system name in green
    printf("\033[0;32m");
    printf("%s:",system_name);

    // directory in cyan
    printf("\033[0;36m");
    printf("%s ", display_directory());

    //reset
    printf("\033[0m");
}