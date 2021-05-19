/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Main Kernel Entry point
 */
#ifndef IDT_H
#define IDT_H

#include "global.h"

/**
 * Interrupt Descriptor Table initialization
 * This adds entries to the IDT and then enables interrupts
 */
void idt_init();

/**
 * Adds an entry to the IDT
 * @param  entry_num - interrupt/entry number
 * @param  func_ptr  - pointer to the function (interrupt service routine) when
 *                     the interrupt is triggered.
 */
void idt_entry_add(int entry_num, func_ptr_t func_ptr);

#endif
