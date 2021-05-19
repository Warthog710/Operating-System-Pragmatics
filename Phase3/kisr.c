/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Interrupt Service Routines
 */

#include "spede.h"
#include "kernel.h"
#include "kisr.h"
#include "ksyscall.h"
#include "kutil.h"
#include "syscall_common.h" // Adding this header file with the syscall definitions

/**
 * Kernel Interrupt Service Routine: Timer (IRQ 0)
 */
void kisr_timer() 
{
    //Increment the system time
    //? Note: system_time / CLK_TCK = system_time in seconds (rounded down)
    system_time++;

    //Increment active time and total time on the active process
    pcb[active_pid].active_time++;
    pcb[active_pid].total_time++;

    // Dismiss IRQ 0 (Timer)
    outportb(0x20, 0x60);
}

void kisr_syscall()
{
    //Switch statement will select the appropriate system call function
    //depending on what system call number was stored in EAX.
    switch(pcb[active_pid].trapframe_p->eax)
    {
        case SYSCALL_GET_SYS_TIME:
            ksyscall_get_sys_time();
            break;
        case SYSCALL_GET_PROC_PID:
            ksyscall_get_proc_pid();
            break;
        case SYSCALL_GET_PROC_NAME:
            ksyscall_get_proc_name();
            break;
        case SYSCALL_SLEEP:
            ksyscall_sleep();
            break;
        case SYSCALL_PROC_EXIT:
            ksyscall_proc_exit();
            break;
        //Throw error if syscall # is invalid
        default:
            panic("Invalid syscall!!!\n");
            break;
    }

}
