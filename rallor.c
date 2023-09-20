#include "shell.h"

/**
 * _custom_memset - Fill memory with a specified byte
 * @mem: Pointer to the memory area
 * @byte: The byte to fill the memory with
 * @size: The number of bytes to be filled
 *
 * Return: A pointer to the memory area `mem`
 */
char *_custom_memset(char *mem, char byte, unsigned int size)
{
    unsigned int index;

    for (index = 0; index < size; index++)
        mem[index] = byte;
    return (mem);
}

/**
 * free_string_array - Free memory occupied by an array of strings and the array itself
 * @str_array: The array of strings to be freed
 */
void free_string_array(char **str_array)
{
    char **temp = str_array;

    if (!str_array)
        return;

    while (*str_array)
        free(*str_array++);
    
    free(temp);
}

/**
 * custom_realloc - Reallocate memory for a block of data
 * @ptr: Pointer to the previous memory block
 * @old_size: Byte size of the previous block
 * @new_size: Byte size of the new block
 *
 * Return: Pointer to the updated memory block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *new_ptr;

    if (!ptr)
        return (malloc(new_size));

    if (!new_size)
        return (free(ptr), NULL);

    if (new_size == old_size)
        return (ptr);

    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);

    old_size = old_size < new_size ? old_size : new_size;

    while (old_size--)
        new_ptr[old_size] = ((char *)ptr)[old_size];
    
    free(ptr);
    return (new_ptr);
}

