#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct proc_node{
    char name[50];
    int pid;
    int status;
    struct proc_node* next;
}proc;

extern proc* head;

void append_proc(proc* proc_new);
void display_procs();
proc* find_proc(char* filter_value, int option);
void remove_proc(proc* job);