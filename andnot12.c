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

        /* Split line into commands using ; as separator */
        char *token = strtok(line, ";");
        while (token != NULL)
        {
            /* Check for exit command */
            if (strncmp(token, "exit", 4) == 0)
            {
                int status = atoi(token + 5);
                free(line);
                exit(status);
            }

            /* Check for env command */
            if (strcmp(token, "env") == 0)
            {
                system("printenv");
                token = strtok(NULL, ";");
                continue;
            }

            /* Check for setenv command */
            if (strncmp(token, "setenv", 6) == 0)
            {
                char *name = strtok(token + 7, " ");
                char *value = strtok(NULL, " ");
                if (setenv(name, value, 1) == -1)
                    fprintf(stderr, "Error setting environment variable\n");
                token = strtok(NULL, ";");
                continue;
            }

            /* Check for unsetenv command */
            if (strncmp(token, "unsetenv", 8) == 0)
            {
                char *name = token + 9;
                if (unsetenv(name) == -1)
                    fprintf(stderr, "Error unsetting environment variable\n");
                token = strtok(NULL, ";");
                continue;
            }

            /* Check for cd command */
            if (strncmp(token, "cd", 2) == 0)
            {
                char *dir = token + 3;
                if (*dir == '\0')
                    dir = getenv("HOME");
                else if (strcmp(dir, "-") == 0)
                    dir = getenv("OLDPWD");

                if (chdir(dir) == -1)
                    fprintf(stderr, "Error changing directory\n");

                setenv("OLDPWD", getenv("PWD"), 1);
                setenv("PWD", getcwd(NULL, 0), 1);

                token = strtok(NULL, ";");
                continue;
            }

            /* Split each command into subcommands using && and || as separators */
            char *subtoken = strtok(token, "&&||");
            while (subtoken != NULL)
            {
                /* Check if subcommand exists in PATH */
                char *dup_path = strdup(path);
                char *s = strtok(dup_path, ":");
                while (s != NULL)
                {
                    int length = strlen(s) + strlen(subtoken) + 2;
                    command = malloc(length);
                    strcpy(command, s);
                    strcat(command, "/");
                    strcat(command, subtoken);

                    if (access(command, F_OK) != -1)
                    {
                        system(command);
                        free(command);
                        break;
                    }
                    s = strtok(NULL, ":");
                    free(command);

