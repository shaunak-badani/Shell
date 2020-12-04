#include "signal_handlers.h"
#include "ll_fns.h"

void child_signal_handler() {
    int status;
    pid_t w_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

    char* str_pid = (char*)malloc(sizeof(char) * 50);
    sprintf(str_pid, "%d", (int)w_pid);

    proc* q = find_proc(str_pid, 1);
    free(str_pid);

    if(q == NULL)
        return;
    if(WIFSTOPPED(status))
        return;
    
    if(WIFEXITED(status))
        printf("\n%s with pid %d exited normally. \n", q->name, w_pid);
    if(WIFSIGNALED(status)) 
        printf("\n%s with pid %d exited with status %d. \n", q->name, w_pid, status);

    remove_proc(q);
}

void child_continuation() {
    pid_t w_pid = waitpid(-1, NULL, WNOHANG | WUNTRACED | WCONTINUED);
    char* str_pid = (char*)malloc(sizeof(char) * 50);

    sprintf(str_pid, "%d", (int) w_pid);
    proc* q = find_proc(str_pid, 1);

    q->status = 1;
    return;
}

void add_all_signal_handlers() {
    signal(SIGCHLD, child_signal_handler);
    signal(SIGINT, SIG_IGN);
}