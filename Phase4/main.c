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
#include "string.h"
#include "kproc.h"
#include "idt.h"
#include "kproc.h"
#include "user_proc.h"

/**
 * Main entry point for our kernel/operating system
 */
int main() 
{
    char os_name[32];

    sp_memset(os_name, 0, sizeof(os_name));
    sp_strncpy(os_name, OS_NAME, sizeof(os_name));

    cons_printf("Welcome to %s!\n", os_name);

    // Initialize kernel data structures
    kernel_init();

    // Initialize the IDT
    idt_init();

    // Launch the kernel idle task, printer task, and dispatcher
    // Pass idle_q as of phase3.0
    kproc_exec("ktask_idle", &ktask_idle, &idle_q);
    kproc_exec("Dispatcher", &dispatcher_proc, &run_q);
    kproc_exec("Printer", &printer_proc, &run_q);


    // Start the process scheduler
    kproc_schedule();

    // should never be reached
    return 0;
}

