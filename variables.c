#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_VAR_NUM 100
#define MAX_VAR_NAME 100
#define MAX_VAR_VALUE 100

typedef struct var {
    char name[MAX_VAR_NAME];
    char value[MAX_VAR_VALUE];
} var_t;

var_t vars[MAX_VAR_NUM];
int var_count = 0;
int last_exit_status = 0;

void add_var(char *name, char *value) {
    int i;
    for (i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            strcpy(vars[i].value, value);
            return;
        }
    }
    strcpy(vars[var_count].name, name);
    strcpy(vars[var_count].value, value);
    var_count++;
}

char *get_var(char *name) {
    int i;
    for (i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    return NULL;
}

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

        /* Check for variable assignment */
        char *name = strtok(line, "=");
        char *value = strtok(NULL, "=");
        if (value != NULL) {
            add_var(name, value);
            continue;
        }

        /* Check for variable replacement */
        if (*name == '$') {
            if (strcmp(name + 1, "?") == 0) {
                printf("%d\n", last_exit_status);
                continue;
            } else if (strcmp(name + 1, "$") == 0) {
                printf("%d\n", getpid());
                continue;
            } else {
                char *var_value = get_var(name + 1);
                if (var_value != NULL) {
                    printf("%s\n", var_value);
                }
                continue;
            }
        }

        /* Other commands... */
    }
}

