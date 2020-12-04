#include "ll_fns.h"
#include "pinfo.h"

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
    pinfo_proc* g = getpinfo(node->pid);
    if(strcmp(g->status, "T") == 0) 
        printf("Stopped ");
    else
        printf("Running ");
    free(g);
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

proc* find_proc(char* filter_value, int option) {

    // option == 0 corresponds to name
    // option == 1 corresponds to process id
    // option == 2 corresponds to index

    proc* cur = head;
    if(option > 2 || option < 0)
        return NULL;
        
    int i = 1;
    while(cur != NULL) {
        if(option == 0 && (strcmp(cur->name, filter_value) == 0))
            break;
        if(option == 1 && atoi(filter_value) == cur->pid)
            break;
        if(option == 2 && atoi(filter_value) == i)
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
