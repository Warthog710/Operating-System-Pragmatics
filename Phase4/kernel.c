/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Main Kernel Entry point
 */

#include "spede.h"
#include "global.h"
#include "kernel.h"
#include "kutil.h"
#include "kisr.h"
#include "user_proc.h"
#include "kproc.h"
#include "string.h"
#include "spede/string.h"

/**
 * Kernel data structures and variables
 * Externed via kernel.h
 */

char        stack[PROC_MAX][PROC_STACK_SIZE];

int         system_time,
            active_pid;

queue_t     available_q, run_q,
            sleep_q,
            idle_q,
            semaphore_q;

pcb_t       pcb[PROC_MAX];
semaphore_t semaphores[SEMAPHORE_MAX];
mailbox_t   mailboxes[MBOX_MAX];

/**
 * Kernel Initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init() 
{
    //Index variables
    int i, j;

    cons_printf("Initializing kernel data structures\n");

    //Initialize active pid. Using non positive int to represent no active PID
    active_pid = -1;

    //Initialize runtime stacks of processes by using nested for loops
    for(i = 0; i < PROC_MAX; i++)
        for(j = 0; j < PROC_STACK_SIZE; j++)
            stack[i][j] = 0;

    //Initialize process table using memset
    sp_memset(pcb, 0, PROC_MAX * sizeof(pcb_t));

    //Initialize system time to 0
    system_time = 0;
    
    //Initialize queues using the queue_init functions
    queue_init(&run_q);
    queue_init(&available_q); 
    queue_init(&sleep_q);
    queue_init(&idle_q);
    queue_init(&semaphore_q);

    //Fill up available_q with process id's
    for(i = 0; i < QUEUE_SIZE; i++)
        queue_in(&available_q, i);

    //Fill up semaphore_q with process id's
    for(i = 0; i < SEMAPHORE_MAX; i++)
    {
        queue_in(&semaphore_q, i);
        semaphores[i].init = SEMAPHORE_UNINITIALIZED;
    }
}

/**
 * Kernel run loop
 *  - Process interrupts
 *  - Handle any "developer" commands
 *  - Run the process scheduler
 *
 * @param  trapframe - pointer to the current trapframe
 */
void kernel_run(trapframe_t *trapframe)
{
    char key;

    // If we do not have a valid PID, then panic
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!!\n");

    // save the trapframe into the PCB of the currently running process
    pcb[active_pid].trapframe_p = trapframe;

    // Process the current interrupt and call the appropriate service routine
    switch (trapframe->interrupt)
    {
        // Timer interrupt
        case TIMER_INTR:
            kisr_timer();
            break;

        // System call
        case SYSCALL_INTR:
            kisr_syscall();
            break;

        default:
            panic("Invalid interrupt!!!\n");
            break;
    }

    // Process special developer/debug commands
    if (cons_kbhit()) 
    {
        key = cons_getchar();

        switch (key)
        {
            case 'b':
                // Set a breakpoint
                breakpoint();
                break;

            case 'n':
                // Create a new process
                kproc_exec("User_proc", &user_proc, &run_q);
                break;

            case 'p':
                // Trigger a panic (aborts)
                panic("PANIC! (>.<)\n");
                break;

            case 'x':
                // Exit the currently running process
                printf("Attempting to exit process %d\n", active_pid);
                kproc_exit(active_pid);
                break;

            case 'q':
                // Exit our kernel
                cons_printf("Exiting!!!\n");
                exit(0);
                break;

            default:
                // Display a warning (no abort)
                panic_warn("Unknown command! (O_o)\n");
                break;
        }
    }

    // Run the process scheduler
    kproc_schedule();
}


