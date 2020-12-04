# include "proc.h"

proc* head;

void initialize_jobs() {
    head = NULL;
    return;
}

void add_job(char* job_name, pid_t process_id) {
    proc* proc_new = (proc*)malloc(sizeof(proc));

    strcpy(proc_new->name, job_name);
    proc_new->pid = (int)process_id;
    proc_new->status = 1;
    proc_new->next = NULL;

    append_proc(proc_new);   
}

void show_jobs() {
    display_procs();
}

void jobs(char p[][50], int i) {
    if(i != 0)
        printf("Usage : jobs\n");
    else
        show_jobs();
}

