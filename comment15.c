#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        printf("#cisfun$ ");
        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(EXIT_SUCCESS);
        }

        /* Remove newline character from line if it exists */
        char *pos;
        if ((pos=strchr(line, '\n')) != NULL)
            *pos = '\0';

        /* Check for comments */
        pos = strchr(line, '#');
        if (pos != NULL)
            *pos = '\0';

        /* Other commands... */
    }
}

