#ifndef SYSCALL_H
#define SYSCALL_H

void proc_exit(void);

int get_sys_time(void);

int get_proc_pid(void);
int get_proc_name(char *name);

void sleep(int seconds);

#endif
