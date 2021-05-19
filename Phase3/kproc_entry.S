/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Process Handling
 */
#include <spede/machine/asmacros.h>
#include "kproc.h"

.text

// Loads a process trapframe so the process can run
ENTRY(kproc_load)
    movl 4(%esp), %eax      // load stack pointer into eax
    movl %eax, %esp         // Point esp to trapframe
    popl %gs                // restore segment registers
    popl %fs
    popl %es
    popl %ds
    popa                    // restore general registers
    add $4, %esp            // skip 4 bytes that stored the interrupt
    iret

