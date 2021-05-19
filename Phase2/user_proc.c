/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * User Processes
 */
#include "global.h"
#include "spede.h"

#include "user_proc.h"

void user_proc() {
    int i;

    // Indicate that process has started
    cons_printf("user_proc started\n");

    // Run loop
    while (1) {
        cons_printf("user_proc running\n");

        // Busy loop/delay for 1s
        for (i = 0; i < IO_DELAY_LOOP; i++) {
            IO_DELAY();
        }
    }
}
