#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h> 
#include <stdbool.h>
#include <time.h>

void print_cwd();
void change_directory(char flags[][50], int size_of_flags);
char* join_dirs(char dir_array[][50], int len_of_array);
