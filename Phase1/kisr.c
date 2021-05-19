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

/**
 * Kernel Interrupt Service Routine: Timer (IRQ 0)
 */
void kisr_timer() 
{
    // Increment the system time
    system_time++;

    // print 'x' to the target console every second
    // Since the timer ticks 100 x per second (defined via CLK_TCK),
    // we should print every 100 ticks
    if (system_time % 100 == 0)
    {
        cons_printf("X");
    }

    // Dismiss IRQ 0 (Timer)
    outportb(0x20, 0x60);
}
