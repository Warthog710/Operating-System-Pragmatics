/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Internal Kernel APIs and Data structures
 */

#ifndef KERNEL_H
#define KERNEL_H

#include "global.h"

// System time
extern int system_time;

/**
 * Function declarations
 */

/*
 * Kernel initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init();

#endif
