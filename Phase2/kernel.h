/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Internal Kernel APIs and Data structures
 */

#ifndef KERNEL_H
#define KERNEL_H

#include "global.h"
#include "queue.h"
#include "trapframe.h"

// Global Definitions

// Maximum process ID possible (0-based PIDs)
#define PID_MAX PROC_MAX-1

// Process runtime stack size
#define PROC_STACK_SIZE 8196

// Maximum number of ticks a process may run before being rescheduled
#define PROC_TICKS_MAX 50

/**
 * Kernel data types and definitions
 */

// Process states
typedef enum {
    AVAILABLE,
    RUNNING,
    ACTIVE
} state_t;

// The process control block for each process
typedef struct {
    char name[PROC_NAME_LEN+1];     // Process name/title
    state_t state;                  // current process state
    queue_t *queue;                 // queue the process belongs to
    int active_time;                // current cpu time while active
    int total_time;                 // total cpu time since created
    trapframe_t *trapframe_p;       // process trapframe
} pcb_t;


/**
 * Kernel data structures - available to the entire kernel
 */

// runtime stacks of processes
extern char stack[PROC_MAX][PROC_STACK_SIZE];

// process table
extern pcb_t pcb[PROC_MAX];

// System time
extern int system_time;

// ID of the actively running process, -1 means not set
extern int active_pid;

// Process queues
extern queue_t available_q;
extern queue_t run_q;


/**
 * Function declarations
 */

/*
 *  * Kernel initialization
 *   * Initializes all kernel data structures and variables
 *    */
void kernel_init();

#endif
