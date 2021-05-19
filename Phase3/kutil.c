/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Internal Kernel APIs
 */
#include "spede.h"

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message on the target console
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits
 * @param msg   the message to display
 */
void panic(char *msg)
{
    // Display a message indicating a panic was hit
    cons_printf(msg);
    cons_printf("FATAL: Activating panic - SAVE OUR SYSTEM!\n");
    // trigger a breakpoint
    breakpoint();
    // Abort since this is a fatal condition!
    // We will use abort() for immediate program termination.
    /* REASONING:
     *  One idea is that a fatal operating system error is unrecoverable. (Like BSOD: reboot required)
     *  Therefore, object/variable cleanup not needed via exit() because memory will be wiped.
     */ 
    abort();
}

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a warning message on the target console
 *   - Triggers a breakpoint (if running through GDB)
 * @param msg   the message to display
 */
void panic_warn(char *msg)
{
    // Display a message indicating a warning was hit
    cons_printf(msg);
    cons_printf("WARN: Activating panic - SAVE OUR SYSTEM!\n");
    // trigger a breakpoint
    breakpoint();
}

