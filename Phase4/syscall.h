#ifndef SYSCALL_H
#define SYSCALL_H

#include "ipc.h"

/*
 * Exits the current process
 */
void proc_exit(void);

/*
 * Gets the current system time (in seconds)
 * @return system time in seconds
 */
int get_sys_time(void);

/*
 * Gets the current process' id
 * @return process id
 */
int get_proc_pid(void);

/*
 * Gets the current process' name
 * @param name - pointer to a character buffer where the name will be copied
 * @return 0 on success, -1 or other non-zero value on error
 */
int get_proc_name(char *name);

/*
 * Puts the current process to sleep for the specified number of seconds
 * @param seconds - number of seconds the process should sleep
 */
void sleep(int seconds);

/*
 * Initializes a semaphore
 * @param sem - pointer to semaphore identifier
 * The semaphore identifier will be set by the kernel
 */
void sem_init(sem_t *sem);

/*
 * Wait on a semaphore
 * @param sem - pointer to semaphore identifier
 *
 * Requires that the semaphore identifier has been initialized
 */
void sem_wait(sem_t *sem);

/*
 * Post a semaphore
 * @param sem - pointer to semaphore identifier
 *
 * Requires that the semaphore identifier has been initialized
 */
void sem_post(sem_t *sem);

/*
 * Send a message
 * @param msg - pointer to the local message data structure
 * @param mbox_num - the mailbox to send the message to
 */
void msg_send(msg_t *msg, int mbox_num);

/*
 * Receive a message
 * @param msg - pointer to the local message data structure
 * @param mbox_num - the mailbox to receive the message from
 */
void msg_recv(msg_t *msg, int mbox_num);

#endif
