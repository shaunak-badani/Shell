#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h> 
#include <stdbool.h>
#include <time.h>

uid_t userId;

char CWD[50][50];
char ENV_HOME[50][50];
int size_of_folder_dirs;
int home_dirs;


// file_listing also requires this function
char* get_name(int id, bool forUser);

// details.c requires this
char* display_directory();


//file listing
void list_flag_handler(char flags[][50], int size_of_flags);

char username[PATH_MAX];
char system_name[PATH_MAX];
