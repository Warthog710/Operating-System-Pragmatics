/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Process Handling
 */
#include "spede.h"
#include "kernel.h"
#include "kutil.h"
#include "kproc.h"
#include "queue.h"
#include "string.h"

/**
 * Process scheduler
 */
void kproc_schedule() 
{
    //Make sure we have an active PID, if not get a new one
    if (active_pid == -1)
    {
        //Make sure we get a valid process
        if (queue_out(&run_q, &active_pid) != 0)
        {
            panic("No tasks scheduled to run!!!\n");
        }
        else
        {
            //If we got a -1 back... panic
            if (active_pid == -1)
            {
                panic("No tasks scheduled to run!!!\n");
            }

            //Set process state to active
            pcb[active_pid].state = ACTIVE;
        }
    }

    //Once the active process has exceeded the maximum
    //number of ticks, it needs to be unscheduled:
    if (pcb[active_pid].active_time > PROC_TICKS_MAX)
    {
        //Reset active time, set state to running, queue out process from run_q
        pcb[active_pid].active_time = 0;
        pcb[active_pid].state = RUNNING;
        if (queue_in(&run_q, active_pid) != 0)
        {
            panic_warn("Failed to queue in active process to running queue\n");
        }

        //Clear active PID
        active_pid = -1;
    }

    //If we don't have an actively running process:
    //queue_out the process from the running queue and set it to the active pid
    //set the state in the process control block for the new active proces to ACTIVE
    if (active_pid == -1)
    {
        //Make sure we get a valid process
        if (queue_out(&run_q, &active_pid) != 0)
        {
            panic("No tasks scheduled to run!!!\n");
        }
        else
        {
            //If we got a -1 back... panic
            if (active_pid == -1)
            {
                panic("No tasks scheduled to run!!!\n");
            }
            
            //Set process state to active
            pcb[active_pid].state = ACTIVE;
        }
    }
    
    // Load the next process
    kproc_load(pcb[active_pid].trapframe_p);
}

/**
 * Start a new process
 * @param proc_name The process title
 * @param proc_ptr  function pointer for the process
 * @param queue     the run queue in which this process belongs
 */
void kproc_exec(char *proc_name, void *proc_ptr, queue_t *queue) 
{
    int pid, index;

    //Ensure that valid parameters have been specified and panic otherwise
    if (proc_name == NULL || proc_ptr == NULL || queue == NULL)
    {
        panic("Invalid parameters sent to kproc_exec()\n");
    }

    //Dequeue the process from the available queue
    if (queue_out(&available_q, &pid) != 0) 
    {
        //If != is returned, queue_out encountered an error
        panic_warn("Unable to retrieve process from available queue\n");
        return;
    }

    //Make sure the pid range is valid
    if (pid < 0 || pid > PROC_MAX)
    {
        panic_warn("Received an invalid PID from available_q\n");
    }

    //Initialize the PCB entry for the process (e.g. pcb[pid])
    pcb[pid].active_time = 0;
    pcb[pid].total_time = 0;

    //Set the process state to RUNNING
    pcb[pid].state = RUNNING;

    //? Note, rest of PCB entries are intilized below.

    //Copy the process name to the PCB
    sp_strncpy(pcb[pid].name, proc_name, PROC_NAME_LEN);

    //Ensure the stack for the process is cleared (e.g. stack[pid])
    for(index = 0; index < PROC_STACK_SIZE; index++)
    {
        stack[pid][index] = 0;
    }

    //Allocate the trapframe data
    pcb[pid].trapframe_p = (trapframe_t *)&stack[pid][PROC_STACK_SIZE - sizeof(trapframe_t)];

    //Set the instruction pointer in the trapframe
    pcb[pid].trapframe_p->eip = (unsigned int)proc_ptr;

    //Set INTR flag
    pcb[pid].trapframe_p->eflags = EF_DEFAULT_VALUE | EF_INTR;

    //Set each segment in the trapframe
    pcb[pid].trapframe_p->cs = get_cs();
    pcb[pid].trapframe_p->ds = get_ds();
    pcb[pid].trapframe_p->es = get_es();
    pcb[pid].trapframe_p->fs = get_fs();
    pcb[pid].trapframe_p->gs = get_gs();

    //Set the process run queue
    pcb[pid].queue = queue;

    //Move the proces into the associated run queue
    if (queue_in(pcb[pid].queue, pid) != 0)
    {
        panic_warn("Failed to queue in new process to running queue\n");
    }
  
    printf("Executed process %s (%d)\n", pcb[pid].name, pid);
}

/**
 * Exit the currently running process
 */
void kproc_exit(int pid) 
{
    char* name;

    //PID 0 should be our kernel idle task. It should never exit.
    //In this case, print a message to the host terminal
    if (pid == 0)
    {
        printf("Cannot exit the idle_task... (>_<)\n");
    }

    //Panic if we have an invalid PID
    else if (pid == -1)
    {
        panic("Tried to exit an invalid process!!!\n");
    }

    //Panic warn if the PID is not running/active; and return
    else if (pcb[pid].state != RUNNING && pcb[pid].state != ACTIVE)
    {
        panic_warn("Attempted to exit a process with an invalid state!!!\n");
    }

    else
    {
        //Clear the PCB for the process and set the process state to AVAILABLE
        //Queue the pid back to the available queue
        //? It may be possible to memset the entire block (pcb[pid])... This was originally by intent but my code failed. :(
        //? Perhaps someone else can look into this.
        name = pcb[pid].name;
        sp_memset(pcb[pid].name, 0, PROC_NAME_LEN);
        pcb[pid].state = AVAILABLE;
        pcb[pid].queue = NULL;
        pcb[pid].active_time = 0;
        pcb[pid].total_time = 0;
        pcb[pid].trapframe_p = NULL;        
        printf("Exited process %s (%d)\n", name, pid);

        if (queue_in(&available_q, pid) != 0)
        {
            panic_warn("Failed to queue in exited process available queue\n");
        }

        //If the pid is the active pid, then clear the active pid and
        //trigger the the process scheduler to schedule a new process
        if (pid == active_pid)
        {
            active_pid = -1;
            kproc_schedule();
        }
    }
}

/**
 * Kernel idle task
 */
void ktask_idle() 
{
    int i;

    // Indicate that the Idle Task has started
    cons_printf("idle_task started\n");

    // Process run loop
    while (1) 
    {
        cons_printf("idle_task running\n");

        // busy loop/delay
        for (i = 0; i < IO_DELAY_LOOP; i++) 
        {
            IO_DELAY();
        }
    }
}
