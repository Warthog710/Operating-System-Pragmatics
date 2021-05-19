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
#include "idt.h"

/**
 * Main entry point for our kernel/operating system
 */
int main() 
{
    char key;
    int i;

    char os_name[32];
    sp_memset(os_name, 0, sizeof(os_name));
    sp_memcpy(os_name, OS_NAME, sizeof(OS_NAME));

    cons_printf("Welcome to %s\n", os_name);

    // Initialize the kernel
    kernel_init();

    // Initialize the IDT
    idt_init();

    // Enable external interrupts
    cons_printf("Enabling interrupts\n");
    asm("sti");

    while (1) 
    {
        cons_putchar('.');

        // Delay for ~1 second
        for (i = 0; i < IO_DELAY_LOOP; i++) 
        {
            IO_DELAY();
        }

        if (cons_kbhit()) 
        {
            key = cons_getchar();

            switch (key) 
            {
                case 'b':
                    // Set a breakpoint
                    breakpoint();
                    break;

                case 'p':
                    // Trigger a panic (aborts)
                    panic("\nPANIC! (>.<)\n");
                    break;

                case 'q':
                    // Exit our kernel
                    cons_printf("\nExiting!!!\n");
                    exit(0);
                    break;

                default:
                    // Display a warning (no abort)
                    panic_warn("\nUnknown command! (O_o)\n");
                    break;
            }
        }
    }

    // should never be reached
    return 0;
}

