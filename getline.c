#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

/**
 * _getline - reads an entire line from standard input
 *
 * Return: a pointer to the line read
 */
char *_getline(void)
{
    static char buffer[BUFFER_SIZE];
    static char *ptr = buffer;
    static char *end;
    char *line;
    ssize_t n;

    if (ptr == end)
    {
        n = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (n <= 0)
            return (NULL);
        end = buffer + n;
        ptr = buffer;
    }

    line = ptr;
    while (ptr < end && *ptr != '\n')
        ptr++;
    if (ptr < end)
        *ptr++ = '\0';
    else
        *end = '\0';

    return (line);
}

