#include <stdio.h>
#include <stdlib.h>
// contains malloc function
#include <unistd.h>
// contains gethostname, getlogin, getcwd
#include <limits.h>
#include <sys/types.h>
#include <string.h> 


uid_t userId;

char CWD[50][50];
char ENV_HOME[50][50];
int size_of_folder_dirs;
int home_dirs;


void initialize_details();
char* display_directory();
void pretty_print();

// functions to handle other jobs
void parse_command(char *gets);

// functions for built in commands
void print_cwd();
void change_directory(char flags[][50], int size_of_flags);


char username[PATH_MAX];
char system_name[PATH_MAX];
// char absolute_path[PATH_MAX];
