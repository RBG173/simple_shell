#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALIAS_NUM 100
#define MAX_ALIAS_NAME 100
#define MAX_ALIAS_VALUE 100

typedef struct alias {
    char name[MAX_ALIAS_NAME];
    char value[MAX_ALIAS_VALUE];
} alias_t;

alias_t aliases[MAX_ALIAS_NUM];
int alias_count = 0;

void add_alias(char *name, char *value) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            strcpy(aliases[i].value, value);
            return;
        }
    }
    strcpy(aliases[alias_count].name, name);
    strcpy(aliases[alias_count].value, value);
    alias_count++;
}

char *get_alias(char *name) {
    int i;
    for (i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            return aliases[i].value;
        }
    }
    return NULL;
}

void print_aliases() {
    int i;
    for (i = 0; i < alias_count; i++) {
        printf("%s='%s'\n", aliases[i].name, aliases[i].value);
    }
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

        /* Check for alias command */
        if (strncmp(line, "alias", 5) == 0)
        {
            char *args = line + 6;
            if (*args == '\0') {
                print_aliases();
                continue;
            }

            char *name = strtok(args, "=");
            char *value = strtok(NULL, "=");
            if (value == NULL) {
                char *alias_value = get_alias(name);
                if (alias_value != NULL) {
                    printf("%s='%s'\n", name, alias_value);
                }
                continue;
            }

            add_alias(name, value);
            continue;
        }

        /* Other commands... */
    }
}

