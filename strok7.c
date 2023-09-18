#include <stdio.h>
#include <stdlib.h>

char *_strtok(char *str, const char *delim)
{
    static char *next_token = NULL;
    char *token;

    if (str == NULL)
    {
        if (next_token == NULL)
            return NULL;
        str = next_token;
    }

    str += strspn(str, delim);
    if (*str == '\0')
    {
        next_token = NULL;
        return NULL;
    }

    token = str;
    str = strpbrk(token, delim);
    if (str == NULL)
        next_token = NULL;
    else
    {
        *str = '\0';
        next_token = str + 1;
    }

    return token;
}

