/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Global Definitions
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#include "local.h"

// Maximum number of processes we will support
#ifndef PROC_MAX
#define PROC_MAX 20
#endif

// Maximum process name length
#ifndef PROC_NAME_LEN
#define PROC_NAME_LEN 32
#endif

// Number of times to loop over IO_DELAY() to delay for one second
#ifndef IO_DELAY_LOOP
#define IO_DELAY_LOOP 1666666
#endif

// void-return function pointer type
typedef void (*func_ptr_t)();

#endif
