#include "shell.h"

/**
 * string_copy - copies a string
 * @destination: the destination buffer
 * @source: the source string
 *
 * Return: pointer to the destination buffer
 */
char *string_copy(char *destination, char *source)
{
    int i = 0;

    if (destination == source || source == NULL)
        return (destination);

    while (source[i])
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';
    return (destination);
}

/**
 * string_duplicate - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *string_duplicate(const char *str)
{
    int length = 0;
    char *duplicate;

    if (str == NULL)
        return (NULL);

    while (*str++)
        length++;

    duplicate = malloc(sizeof(char) * (length + 1));
    if (!duplicate)
        return (NULL);

    for (length++; length--;)
        duplicate[length] = *--str;

    return (duplicate);
}

/**
 * print_string - prints a string to stdout
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void print_string(char *str)
{
    int i = 0;

    if (!str)
        return;

    while (str[i] != '\0')
    {
        write_character(str[i]);
        i++;
    }
}

/**
 * write_character - writes a character to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_character(char c)
{
    static int index;
    static char buffer[WRITE_BUFFER_SIZE];

    if (c == FLUSH_BUFFER || index >= WRITE_BUFFER_SIZE)
    {
        write(1, buffer, index);
        index = 0;
    }

    if (c != FLUSH_BUFFER)
        buffer[index++] = c;

    return (1);
}

