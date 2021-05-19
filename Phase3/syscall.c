#include "syscall.h"
#include "syscall_common.h"
#include "kernel.h"
#include "spede.h"


/*
 * Anatomy of a system call
 *
 * Data to/from the kernel is exchanged via the CPU trapframe
 * The system call is triggered via an interrupt that the kernel processes
 * Data can be sent to and received from the kernel in a single system call
 *
 * Data from the kernel:
 * int MySyscall() {
 *     int x;
 *     asm("movl %1, %%eax;"    // eax register indicates the syscall
 *         "int $0x80;"         // trigger the syscall
 *         "movl %%ebx, %0;"    // pull data back from the kernel via register
 *         : "=g" (x)           // operand 0 is data returned from the kernel
 *         : "g" (SYSCALL_FOO)  // operand 1 is the syscall
 *         : "eax", "ebx");     // restore the registers that were used
 *     return x;
 * }
 *
 * Data to the kernel:
 * void MySyscall(int x) {
 *     int x;
 *     asm("movl %0, %%eax;"    // eax register indicates the syscall
 *         "movl %1, %%ebx;"    // send data to the kernel
 *         "int $0x80;"         // trigger the syscall
 *         :                    // no operands for return data
 *         : "g" (SYSCALL_FOO), // operand 0 is the syscall
 *           "g" (x)            // operand 1 is the data we are sending
 *         : "eax", "ebx");     // restore the registers that were used
 * }
 *
 * Data to and from the kernel:
 * int MySyscall(int x) {
 *     int y;
 *     asm("movl %1, %%eax;"    // eax register indicates the syscall
 *         "movl %2, %%ebx;"    // send data to the kernel
 *         "int $0x80;"         // trigger the syscall
 *         "movl %%ebx, %3;"    // pull data back from the kernel via register
 *         : "=g" (y)           // operand 0 is data returned from the kernel
 *         : "g" (SYSCALL_FOO), // operand 1 is the syscall
 *           "g" (x)            // operand 2 is the data we are sending
 *         : "eax", "ebx");     // restore the registers that were used
 *     return y;
 * }
 */

void proc_exit()
{
    asm
    (
        "movl %0, %%eax;"                       // eax register indicates the syscall
        "movl %1, %%ebx;"                       // send data to the kernel
        "int $0x80;"                            // trigger the syscall
        :                                       // no operands for return data
        : "g" (SYSCALL_PROC_EXIT), "g" (0)      // operand 0 is the syscall, operand 1 is the data we are sending
        : "eax", "ebx"                          // restore the registers that were used
    );
}

int get_sys_time()
{
    int time = -1;

    asm
    (
        "movl %1, %%eax;"                       // eax register indicates the syscall
        "int $0x80;"                            // trigger the syscall
        "movl %%ebx, %0;"                       // pull data back from the kernel via register
        : "=g" (time)                           // operand 0 is data returned from the kernel
        : "g" (SYSCALL_GET_SYS_TIME)            // operand 1 is the syscall
        : "eax", "ebx"                          // restore the registers that were used
    );

    // Return time to calling process
    return time;
}

int get_proc_pid()
{
    int pid = -1;

    asm("movl %1, %%eax;"    // eax register indicates the syscall
          "int $0x80;"           // trigger the syscall
          "movl %%ebx, %0;"    // pull data back from the kernel via register
          : "=g" (pid)           // operand 0 is data returned from the kernel
          : "g" (SYSCALL_GET_PROC_PID)  // operand 1 is the syscall
          : "eax", "ebx");     // restore the registers that were used
    
    if(pid < 0 || pid > PID_MAX)
    {
        return -1;
    }

    return pid;
}

int get_proc_name(char *name)
{
    int rc = -1;

    asm("movl %1, %%eax;"    // eax register indicates the syscall
          "movl %2, %%ebx;"    // send data to the kernel
          "int $0x80;"          // trigger the syscall
          "movl %%ecx, %0;"
          : "=g" (rc)                   // no operands for return data
          : "g" (SYSCALL_GET_PROC_NAME), // operand 0 is the syscall
            "g" (name)            // operand 1 is the data we are sending
          : "eax", "ebx", "ecx");     // restore the registers that were used

    return rc;
}

void sleep(int seconds)
{
    //Perform the system call
    asm
    (
        "movl %1, %%eax;"                       //Store the system call type in EAX
        "movl %0, %%ebx;"                       //Move the desired seconds into EBX
        "int $0x80;"                            //Trigger syscall interrupt
        :                                       //No return type
        : "g" (seconds), "g" (SYSCALL_SLEEP)    //%0 = desired seconds, %1 = system call type
        : "eax", "ebx"                          //Restore EAX and EBX
    );
}

