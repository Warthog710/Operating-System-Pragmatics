#ifndef KSYSCALL_H
#define KSYSCALL_H

/* System information */
void ksyscall_get_sys_time();

/* Process information */
void ksyscall_get_proc_pid();
void ksyscall_get_proc_name();

/* Additional functionality */
void ksyscall_sleep();

void ksyscall_proc_exit();

#endif
