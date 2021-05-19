/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Interrupt Service Routines
 */
#include <spede/machine/asmacros.h>
#include "kisr.h"

// define kernel stack space
.comm kstack, KSTACK_SIZE, 1
.text

// Timer ISR Handler
ENTRY(kisr_entry_timer)
    // Indicate that the timer interrupt occurred
    pushl $TIMER_INTR
    // Run the common interrupt return routine
    jmp kisr_entry_return

ENTRY(kisr_entry_syscall)
    // Indicate that the system call interrupt occurred
    pushl $SYSCALL_INTR
    // Run the common interrupt return routine
    jmp kisr_entry_return

// Common kernel interrupt return
kisr_entry_return:
    pusha                   // save general registers
    pushl %ds               // save segment registers
    pushl %es
    pushl %fs
    pushl %gs
    movl %esp, %edx
    cld                     // clear the direction flag
    movw $(KDATA), %ax      // load the stack
    mov %ax, %ds
    mov %ax, %es
    leal kstack + KSTACK_SIZE, %esp
    pushl %edx
    call CNAME(kernel_run)  // Run the kernel
