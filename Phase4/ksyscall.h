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

/* Semaphores */
void ksyscall_sem_init();
void ksyscall_sem_wait();
void ksyscall_sem_post();

/* Message Passing */
void ksyscall_msg_send();
void ksyscall_msg_recv();

#endif
