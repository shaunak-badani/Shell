#include "main.h"
#include "utils.h"
#include "prompt.h"
#include "signal_handlers.h"

int main() {
    initialize_details();
    pretty_print();

    size_t bufsize = 10000;
    char* command;
    add_all_signal_handlers();

    int no_of_chars_read = getline(&command, &bufsize, stdin);
    remove_newline(command, no_of_chars_read);

    while(strcmp(command, "quit") != 0) {
        parse_command(command);
        pretty_print();
        no_of_chars_read = getline(&command, &bufsize, stdin);
        remove_newline(command, no_of_chars_read);
    }
}