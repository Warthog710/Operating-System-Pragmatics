#include "spede.h"
#include "kernel.h"
#include "kproc.h"
#include "kutil.h"
#include "string.h"
#include "queue.h"
#include "ksyscall.h"

/**
 * System call kernel handler: get_sys_time
 * Returns the current system time (in seconds)
 */
void ksyscall_get_sys_time()
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Copy the system time from the kernel to the
    // eax register via the running process' trapframe
    pcb[active_pid].trapframe_p->ebx = (system_time / CLK_TCK);
}

/**
 * System call kernel handler: get_proc_id
 * Returns the currently running process ID
 */
//TODO: @jVega45 - Implement ksyscall_get_proc_pid().
void ksyscall_get_proc_pid() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Copy the running pid from the kernel to the
    // ebx register via the running process' trapframe
    pcb[active_pid].trapframe_p->ebx = active_pid;
}

/**
 * System call kernel handler: get_proc_name
 * Returns the currently running process name
 */
//TODO: @jVega45 - Implement ksyscall_get_proc_name().
void ksyscall_get_proc_name() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Set the pointer to the address passed in via EBX
    // Copy the string name from the PCB to the destination
    sp_strncpy((char*)pcb[active_pid].trapframe_p->ebx, pcb[active_pid].name, PROC_NAME_LEN);

    // Indicate success or failure via a return code
    // Set the return code
    if((char*)pcb[active_pid].trapframe_p->ebx == NULL)
    {
        pcb[active_pid].trapframe_p->ecx = -1;
    }
    else
    {
        // return of zero for valid name
        pcb[active_pid].trapframe_p->ecx = 0;
    }
}

/**
 * System call kernel handler: sleep
 * Puts the currently running process to sleep
 */
void ksyscall_sleep() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Calculate the wake time for the currently running process
    pcb[active_pid].wake_time = pcb[active_pid].trapframe_p->ebx + (system_time / CLK_TCK);

    // Move the currently running process to the sleep queue
    queue_in(&sleep_q, active_pid);

    // Change the running process state to SLEEPING
    pcb[active_pid].state = SLEEPING;

    // Clear the running PID so the process scheduler will run
    active_pid = -1;
}

void ksyscall_proc_exit()
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");
    // Trigger the process to exit
    kproc_exit(active_pid);
}
