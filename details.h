#include <stdio.h>
#include <stdlib.h>
// contains malloc function
#include <unistd.h>
// contains gethostname, getlogin, getcwd
#include <limits.h>
#include <sys/types.h>
#include <string.h> 


uid_t userId;
void initialize_details();
void pretty_print();

char username[PATH_MAX];
char system_name[PATH_MAX];
// char absolute_path[PATH_MAX];
