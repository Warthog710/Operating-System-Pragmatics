/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * String Utilities
 */

#include "string.h"
#include "spede/stdio.h"
#include "spede/flames.h"

/**
 * Sets the first n bytes pointed to by str to the value specified by c
 *
 * @param   dest - pointer the block of memory to set
 * @param   c    - value to set; passed as an integer but converted to unsigned
 * char when set
 * @param   n    - number of bytes to set
 * @return  pointer to the memory region being set; NULL on error
 */
void *sp_memset(void *dest, int c, size_t n) 
{
    unsigned char* ptr;

    if(dest == NULL)
    {
        cons_printf(">>>invalid ptr!<<<");
        return NULL;
    }

    ptr = dest; // convert the void ptr to a ptr to an unsigned char

    while(n > 0)
    {
        *ptr = (unsigned char)c; // cannot dereference a void pointer, therefore use the ptr variable
        ptr++;
        n--;
    }  

    return ptr;
}

/**
 * Copies n bytes from the block of memory pointed to by src to dest
 *
 * @param   dest - pointer to the destination block of memory
 * @param   src  - pointer to the source block of memory
 * @param   n    - number of bytes to read/set
 * @return  pointer to the destination memory region; NULL on error
 */
void *sp_memcpy(void *dest, const void *src, size_t n) 
{
    char* cpySrc;
    char* cpyDest;
    int i;

    if((dest == NULL) || (src == NULL))
    {
        return NULL;
    }
    // typecast src and dest addresses to pointers to chars
    cpySrc = (char*)src;
    cpyDest = (char*)dest;

    for(i = 0; i < n; i++)
    {
        cpyDest[i] = cpySrc[i]; // copy src to dest
    }

    return dest;
}

/**
 * Copies the string pointed to by src to the destination dest
 *
 * @param  dest - pointer to the destination string
 * @param  src  - pointer to the source string
 * @return pointer to the destination string
 */
char *sp_strcpy(char *dest, const char *src)
{
    int index = 0;
    char *ptr = dest;// ptr variable to point to destination

    // check for NULL ptr
    if((dest == NULL) || (src == NULL))
    {
        return NULL;
    }
    // copy C-string from src to dest until NULL terminating character is reached
    while(src[index] != '\0') 
    {
        *dest = src[index];
        dest++;
        index++;
    }
   *dest = '\0';  // add the null terminating character to end of dest
   return ptr;
}

/**
 * Copies up to n characters from the source string src to the destination
 * string dest. If the length of src is less than that of n, the remainder
 * of dest up to n will be filled with NULL characters.
 *
 * @param  dest - pointer to the destination string
 * @param  src  - pointer to the source string
 * @param  n    - maximum number of characters to be copied (dest buffer size please!)
 * @return pointer to the destination string
 */
char *sp_strncpy(char *dest, const char *src, size_t n)
{
    int index;
    char *ptr = dest;
    //  check for NULL ptrs
    if((dest == NULL) || (src == NULL))
    {
        return NULL;
    }
    // copy from src until NULL terminating character is reached in src
    for(index = 0; index < (n - 1); index++)
    {
        if(src[index] == '\0')
            break;
        *dest = src[index];
        dest++;
    }
    *dest = '\0';
    return ptr;
}

/**
 * Computes the length of the string str up to, but not including the null
 * terminating character
 *
 * @param  str - pointer to the string
 * @return length of the string
 */
size_t sp_strlen(const char *str)
{

    
    size_t strLength = 0;
    if(str == NULL)
    {
        return strLength;
    }

    // update the strLength variable to increment through src
    while(str[strLength] != '\0')
    {
        strLength++;
    }
    return strLength;
}
