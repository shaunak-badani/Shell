#include "ll_fns.h"

void append_proc(proc* proc_new) {
    proc* cur = head;
    if(cur == NULL) {
        head = proc_new;
        return;
    }
    while(cur->next != NULL)
        cur = cur->next;
    cur->next = proc_new;
    return;
}

void print_proc(proc* node) {
    if(node->status > 0) {
        printf("Running ");
    }
    else {
        printf("Stopped ");
    }
    printf("%s", node->name);
    printf(" [%d]", node->pid);
}

void display_procs() {
    proc* cur = head;
    int i = 1;
    while(cur != NULL) {
        printf("[%d] ", i++);
        print_proc(cur);
        printf("\n");
        cur = cur->next;
    }
}

proc* find_proc(pid_t pid) {
    proc* cur = head;
    
    while(cur != NULL) {
        if(cur->pid == (int)pid)
            break;
        cur = cur->next;
    }
    return cur;
}

void remove_proc(proc* q) {
    proc* cur = head;
    if(head == q){
        head = head->next;
        free(cur);
    }
    
    if(cur == NULL)
        return;
    while(cur->next != NULL) {
        if(cur->next == q)
            break;
        cur = cur->next;
    }
    if(cur->next == NULL)
        return;
    proc* t = cur->next;
    cur->next = t->next;
    free(t);
    return;    
}
