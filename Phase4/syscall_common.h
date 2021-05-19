/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Common system call definitions
 */
#ifndef SYSCALL_COMMON_H
#define SYSCALL_COMMON_H

// Syscall definitions
typedef enum {
    SYSCALL_GET_SYS_TIME,
    SYSCALL_GET_PROC_PID,
    SYSCALL_GET_PROC_NAME,
    SYSCALL_SLEEP,
    SYSCALL_PROC_EXIT,
    SYSCALL_SEM_INIT,
    SYSCALL_SEM_WAIT,
    SYSCALL_SEM_POST,
    SYSCALL_MSG_SEND,
    SYSCALL_MSG_RECV
} syscall_t;

#endif
