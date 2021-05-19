/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * SPEDE Common Definitions
 */
#ifndef SPEDE_H
#define SPEDE_H

// IO_DELAY(), breakpoint(), cons_putchar(), cons_getchar(), cons_kbhit(),
#include <spede/flames.h>

// __BEGIN_DECLS, __END_DECLS, etc.
#include <spede/sys/cdefs.h>

// printf(), sprintf(), vprintf(), sscanf(), etc.
#include <spede/stdio.h>

// inportb(), outportb(), inportw(), etc.
#include <spede/machine/io.h>

// get_idt_base(), get_cs(), set_cr3(), etc
#include <spede/machine/proc_reg.h>

// struct i386_gate, fill_gate(), etc.
#include <spede/machine/seg.h>

// ASM(), CNAME(), ENTRY(), etc.
#include <spede/machine/asmacros.h>

#endif
