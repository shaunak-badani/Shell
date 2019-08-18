#include "details.h"
#include <sys/stat.h>
#include <fcntl.h> 
// contains all the macros

uid_t userId;

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
    char* temp = (char*)malloc(sizeof(char) * 1000);
    char* uid = (char*)malloc(sizeof(char) * 100);
    int len = sizeof(file_lines) / sizeof(char);
    j--;
    for( ; j >= 0 ; j--) {
        user = strtok(file_lines[j],":");
        temp = strtok(NULL,":");
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
}


// Function to print the prompt in the format required
void pretty_print() {
    printf("<%s@%s:~>", username, system_name);
}