/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * IDT Functions
 */

#include "spede.h"
#include "global.h"
#include "kisr.h"
#include "kutil.h"
#include "idt.h"

// Interrupt descriptor table
struct i386_gate *idt_p;

/**
 * Interrupt Descriptor Table initialization
 * This adds entries to the IDT and then enables interrupts
 */
void idt_init() {
    cons_printf("Initializing the IDT\n");

    // Get the IDT base address
    idt_p = get_idt_base();

    // Add an entry for each interrupt into the IDT
    idt_entry_add(TIMER_INTR, kisr_entry_timer);
    
    // This next one enters an interrupt for the system call into the IDT
    idt_entry_add(SYSCALL_INTR, kisr_entry_syscall);

    // Clear the PIC mask to enable interrupts
    outportb(0x21, ~1);
}

/**
 * Adds an entry to the IDT
 * @param  entry_num - interrupt/entry number
 * @param  func_ptr  - pointer to the function (interrupt service routine) when
 *                     the interrupt is triggered.
 */
void idt_entry_add(int entry_num, func_ptr_t func_ptr) {
    struct i386_gate *gateptr;

    // Ensure that we have a valid entry
    if (entry_num < 0 || entry_num > 255) {
        panic("Invalid interrupt\n");
    }

    if (func_ptr == NULL) {
        panic("Invalid function pointer\n");
    }

    // Obtain the gate pointer for the entry
    gateptr = &idt_p[entry_num];

    // Fill the gate
    fill_gate(gateptr, (int)func_ptr, get_cs(), ACC_INTR_GATE, 0);
}

