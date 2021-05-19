/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Internal Kernel APIs and Data structures
 */

#ifndef SYSCALL_COMMON_H
#define SYSCALL_COMMON_H

// Syscall definitions
typedef enum {
    SYSCALL_GET_SYS_TIME,
    SYSCALL_GET_PROC_PID,
    SYSCALL_GET_PROC_NAME,
    SYSCALL_SLEEP,
    SYSCALL_PROC_EXIT
} syscall_t;

#endif
