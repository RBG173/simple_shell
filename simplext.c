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
    char *path = getenv("PATH");
    char *command = NULL;

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

        /* Check for exit command */
        if (strncmp(line, "exit", 4) == 0)
        {
            int status = atoi(line + 5);
            free(line);
            exit(status);
        }

        /* Check for env command */
        if (strcmp(line, "env") == 0)
        {
            system("printenv");
            continue;
        }

        /* Check if command exists in PATH */
        char *dup_path = strdup(path);
        char *s = strtok(dup_path, ":");
        while (s != NULL)
        {
            int length = strlen(s) + strlen(line) + 2;
            command = malloc(length);
            strcpy(command, s);
            strcat(command, "/");
            strcat(command, line);

            if (access(command, F_OK) != -1)
            {
                system(command);
                free(command);
                break;
            }
            s = strtok(NULL, ":");
            free(command);
        }
        free(dup_path);
    }
    return (0);
}

