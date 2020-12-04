#include "signal_handlers.h"
#include "ll_fns.h"

void child_signal_handler() {
    int status;
    pid_t w_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    proc* q = find_proc(w_pid);
    
    if(WIFEXITED(status))
        printf("%s with pid %d exited normally. \n", q->name, w_pid);
    else
        printf("%s with pid %d exited with status %d \n", q->name, w_pid, status);

    remove_proc(q);

}

void add_all_signal_handlers() {
    signal(SIGCHLD, child_signal_handler);
}