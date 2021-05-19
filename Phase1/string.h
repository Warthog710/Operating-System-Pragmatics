/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * String Utilities
 */

#ifndef STRING_H
#define STRING_H

#ifndef _SIZE_T_
typedef __SIZE_TYPE__ size_t;
#endif

/**
 * Sets the first n bytes pointed to by str to the value specified by c
 *
 * @param   dest - pointer the block of memory to set
 * @param   c    - value to set; passed as an integer but converted to unsigned
 * char when set
 * @param   n    - number of bytes to set
 * @return  pointer to the memory region being set; NULL on error
 */
void *sp_memset(void *dest, int c, size_t n);

/**
 * Copies n bytes from the block of memory pointed to by src to dest
 *
 * @param   dest - pointer to the destination block of memory
 * @param   src  - pointer to the source block of memory
 * @param   n    - number of bytes to read/set
 * @return  pointer to the destination memory region; NULL on error
 */
void *sp_memcpy(void *dest, const void *src, size_t n);

#endif
