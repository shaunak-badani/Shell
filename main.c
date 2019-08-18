#include "details.h"

int main() {
    initialize_details();
    pretty_print();

    char command[10000];
    gets(command);
    while(strcmp(command, "quit") != 0) {
        printf("%s\n", command);
        pretty_print();
        gets(command);
    }
}