#include "ll_fns.h"

void initialize_jobs();
void jobs(char p[][50], int i);

void add_job(char* name, pid_t process_id);
int remove_job(int process_no);
void remove_job_pid(pid_t pid);
void stop_job(pid_t pid);
void overkill();
void kill_job_handler(char flags[][50], int i);
void show_jobs();
void fg(char flags[][50], int i);
void bg(char flags[][50], int i);