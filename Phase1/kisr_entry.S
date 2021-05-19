/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Interrupt Service Routines
 */
#include <spede/machine/asmacros.h>
#include "kisr.h"

// Timer ISR Handler
ENTRY(kisr_entry_timer)
    pusha                   // save general registers
    cld                     // Clear the direction flag
    call CNAME(kisr_timer)  // Run the ISR
    popa                    // restore general registers
    iret                    // return

