#include "main.h"
#include <dirent.h>
#include <sys/stat.h>
// for all stat on files

/* Function declarations */
void list_files(int mode, char* directory);
char determine_type(struct dirent *dr);
char* format_permissions(struct stat st);

/* Middleware for ls command */
void list_flag_handler(char flags[][50], int size_of_flags) {

    /* decides the mode of file listing to be done */
    if(size_of_flags == 0) {

        // normal ls command without flags : 
        list_files(0, ".");
        return;
    }
    int mode = 0;
    if(size_of_flags == 1) {
        if(flags[0][0] == '-') {
            int len = strlen(flags[0]);
            for(int i = 1; i < len ; i++) {
                switch(flags[0][i]) {
                    case 'a':
                        mode |= 1;
                        break;
                    case 'l':
                        mode |= 2;
                        break;
                    default:
                        printf("Unrecognized argument : %c \n", flags[0][i]);
                        break;
                }
            }
        } else {
            // ls <directory>
            list_files(mode, flags[0]);
            return;
        }
    }
    else if(size_of_flags > 1) {
        for(int i = 1; i < size_of_flags ; i++) {
            if(flags[i][0] != '-') {
                printf("Please precede your flags with a '-'.\n Quitting \n");
            }
            else {
                int len = strlen(flags[i]);
                for(int j = 1 ; j < len ; j++) {
                        switch(flags[j][0]) {
                        case 'a':
                            mode += 1;
                            break;
                        case 'l':
                            mode += 2;
                            break;
                        default:
                            printf("Unrecognized argument : %c \n", flags[j][0]);
                            break;
                    }
                }
            }
        }
    }
    if(mode > 3) {
        printf("Supported arguments : \n -l, -a, -la, -al, <dir-name> \n");
        return;
    }
    else {
        list_files(mode, ".");
        return;
    }
}

/* ls short listing format */
void ls_short(char *directory, bool hidden_files) {
    struct dirent *de;
    DIR *dr = opendir(directory);
    if(dr == NULL) {
        perror("Could not open directory");
        return;
    }
    while ((de = readdir(dr)) != NULL){
        if(!hidden_files && de->d_name[0] == '.') continue;

        if(determine_type(de) == 'd') printf("\033[0;34m");   
        printf("%s\n", de->d_name);
        printf("\033[0m");
    }
    closedir(dr);
    return;
}

/* ls -l */
void ls_long(char* directory, bool hidden_files) {
    struct dirent *de;
    DIR *dr = opendir(directory);
    if(dr == NULL) {
        perror("Could not open directory");
        return;
    }
    while ((de = readdir(dr)) != NULL){
        // ignoring hidden files and folders
        if(!hidden_files && de->d_name[0] == '.') continue;

        struct stat st;
        if(stat(de->d_name, &st) < 0) {
            perror("There was an error fetching stats for current file / folder ");
            return;
        }
        
        // dr-xr-xr-x 
        char type_char = determine_type(de);
        printf("%c", type_char);
        char* perms = format_permissions(st);
        printf("%s", perms);

        // the number following = no. of hard links
        printf("%*d ", 3, (int)st.st_nlink);

        char* user_file = get_name((int)st.st_uid, true);
        printf("%-*s", 17, user_file);
        free(user_file);
        
        char* group_file = get_name((int)st.st_gid, false);
        printf("%-*s", 17, group_file); 

        printf("%*d ", 7, (int)st.st_size);

        // last modified time
        struct tm* broken_time = localtime(&st.st_mtime);
        char formatted_time[40];
        if(strftime(formatted_time, sizeof(formatted_time), "%b %d %R", broken_time)) {
            printf("  %s", formatted_time);
        }
        else{
            printf(" ERR ");
        }

        // name of folder
        if(type_char == 'd') printf("\033[0;34m");
        printf("%*s\n", 20, de->d_name);
        printf("\033[0m");
    }
    closedir(dr);
    return;
}

void list_files(int mode, char* directory) {
    switch(mode) {
        case 0: 
        // normal listing
            ls_short(directory, false);
            return;
        case 1:
        // with flag a
            ls_short(directory, true);
            return;
        case 2:
        // with flag l
            ls_long(directory, false);
            return;
        case 3:
        // with flag la
            ls_long(directory, true);
            return;
        default:
            printf("Please don't abuse this cli \n");
            return;
    }
}

char* format_permissions(struct stat st) {

    char* permissions = (char*) malloc(sizeof(char) * 9);
    mode_t perm = st.st_mode;
    memset(permissions, '\0', 9*sizeof(char));
    // user permissions
    strcat(permissions, st.st_mode & S_IRUSR ? "r" : "-");
    strcat(permissions, st.st_mode & S_IWUSR ? "w" : "-");
    strcat(permissions, st.st_mode & S_IXUSR ? "x" : "-");
    
    // group permissions
    strcat(permissions, st.st_mode & S_IRGRP ? "r" : "-");
    strcat(permissions, st.st_mode & S_IWGRP ? "w" : "-");
    strcat(permissions, st.st_mode & S_IXGRP ? "x" : "-");

    // other permissions
    strcat(permissions, st.st_mode & S_IROTH ? "r" : "-");
    strcat(permissions, st.st_mode & S_IWOTH ? "w" : "-");
    strcat(permissions, st.st_mode & S_IXOTH ? "x" : "-");
    return permissions;
}

// helper functions
char determine_type(struct dirent *dr) {
    switch(dr->d_type) {
        case DT_REG:
            return '-';
        case DT_DIR:
            return 'd';
        default:
            return '?';
    }
}