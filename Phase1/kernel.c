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

// Local function definitions
void kdata_init();

/**
 * Kernel data structures and variables
 * Externed via kernel.h
 */

// Current system time
int system_time;


/**
 * Kernel Initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init() 
{
    cons_printf("Initializing kernel data structures\n");

    // Initialize system time
    system_time = 0;
}

