# include "proc.h"

proc* head;


// Functionality related
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



void kill_job(int index, int signal) {
    char* str_index = (char*)malloc(sizeof(char) * 50);
    sprintf(str_index, "%d", index);
    proc* q = find_proc(str_index, 2);

    if(q == NULL) {
        perror("Index out of bounds \n.");
        return;
    }
    kill(q->pid, signal);
    return;
}

// Command related
void jobs(char p[][50], int i) {
    if(i != 0)
        printf("Usage : jobs\n");
    else
        show_jobs();
}

void kjob(char p[][50], int i) {
    if(i != 2)
        printf("Usage : kjob <job number> <signal number>. \n");
    else {
        int job_number = atoi(p[0]);
        int signal = atoi(p[1]);

        if(!job_number) {
            printf("Not a valid job number. \n");
            return;
        }
        if(!signal) {
            printf("Not a valid signal. \n");
            return;
        }
        kill_job(job_number, signal);
    }
}

void overkill(char p[][50], int i) {
    if(i > 0) {
        printf("Usage : overkill\n");
        return;
    }
    proc* cur = head;
    proc* tmp;
    while(cur != NULL) {
        tmp = cur->next;
        kill(cur->pid, SIGKILL);
        cur = tmp;
    }
    while(cur != NULL) {
        tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    return;
}


