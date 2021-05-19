/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Kernel Process Handling
 */
#ifndef KPROC_H
#define KPROC_H

#ifndef ASSEMBLER
#include "queue.h"
#include "trapframe.h"

// Kernel process functions
void kproc_schedule();
void kproc_load(trapframe_t *trapframe);
void kproc_exec(char *proc_name, void *func_ptr, queue_t *queue);
void kproc_exit(int pid);

// Kernel tasks
void ktask_idle();

/* Defined in kproc_entry.S */
__BEGIN_DECLS

// Process loader
extern void kproc_load();

__END_DECLS
#endif
#endif
