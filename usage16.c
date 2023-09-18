#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - Entry point
 *
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *file;

    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Cannot open file %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    } else {
        file = stdin;
    }

    while (1)
    {
        if (file == stdin) {
            printf("#cisfun$ ");
        }
        read = getline(&line, &len, file);
        if (read == -1)
        {
            free(line);
            exit(EXIT_SUCCESS);
        }

        /* Remove newline character from line if it exists */
        char *pos;
        if ((pos=strchr(line, '\n')) != NULL)
            *pos = '\0';

        /* Other commands... */
    }
}

