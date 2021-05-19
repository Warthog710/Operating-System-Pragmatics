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
#include "syscall_common.h"
#include "ipc.h"

// Global Definitions

// Maximum process ID possible (0-based PIDs)
#define PID_MAX PROC_MAX-1

// Process runtime stack size
#define PROC_STACK_SIZE 8196

// Maximum number of ticks a process may run before being rescheduled
#define PROC_TICKS_MAX 50

// Maximum number of semaphores
#define SEMAPHORE_MAX PROC_MAX

// Maximum number of mailboxes
#define MBOX_MAX PROC_MAX

// Size of each mailbox
#define MBOX_SIZE PROC_MAX


/**
 * Kernel data types and definitions
 */

// Process states
typedef enum {
    AVAILABLE,
    RUNNING,
    ACTIVE,
    SLEEPING,
    WAITING
} state_t;


// The process control block for each process
typedef struct {
    char name[PROC_NAME_LEN+1];     // Process name/title

    state_t state;                  // current process state
    queue_t *queue;                 // queue the process belongs to

    int active_time;                // current cpu time while active
    int total_time;                 // total cpu time since created
    int wake_time;                  // time that the process should "wake up"

    trapframe_t *trapframe_p;       // process trapframe
} pcb_t;


// Semaphore data structure
typedef struct {
    int count;                      // Semaphore count
    int init;                       // Indicates if initialized
    queue_t wait_q;                 // Wait queue for the semaphore
} semaphore_t;


// Mailbox data structures
typedef struct {
    msg_t messages[MBOX_SIZE];      // Incoming messages
    int head;                       // First message
    int tail;                       // Last message
    int size;                       // Total messages
    queue_t wait_q;                 // Processes waiting for messages
} mailbox_t;


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
extern queue_t idle_q;
extern queue_t sleep_q;

// Semaphore Data Stuctures
extern semaphore_t semaphores[SEMAPHORE_MAX];
extern queue_t semaphore_q;

// Mailbox Data Structures
extern mailbox_t mailboxes[MBOX_MAX];

/**
 * Function declarations
 */

/**
 * Kernel initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init();
#endif
