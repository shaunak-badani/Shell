#include "details.h"

/* Apart from parsing the command, this function also acts as middleware for 
   every command that is input into the shell
*/
void parse_command(char *comm){
    char* main_comm = strtok(comm, " ");
    char *flag = main_comm;
    // printf("%s \n", main_comm);

    char flags[50][50];
    // char *temp = (char *)malloc(sizeof(char) * 1000);
    int i = 0;
    while(flag != NULL) {
        // printf("%s\n", flag);
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
}