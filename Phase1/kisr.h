/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Interrupt Service Routines
 */
#ifndef KISR_H
#define KISR_H

/**
 * Interrupt Service Routine definitions
 */

// Interrupt definitions
#define TIMER_INTR 0x20

#ifndef ASSEMBLER
/**
 * Function declarations
 */

// Timer ISR
void kisr_timer();

/* Defined in kisr_entry.S */
__BEGIN_DECLS

// Kernel interrupt entries
extern void kisr_entry_timer();

__END_DECLS
#endif
#endif
