#include "shell.h"

/**
 * is_chain_delimiter - checks if the current character in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buf: the character buffer
 * @p: address of the current position in buf
 *
 * Return: 1 if it's a chain delimiter, 0 otherwise
 */
int is_chain_delimiter(info_t *info, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|')
    {
        buf[j] = '\0';
        j++;
        info->command_buffer_type = CMD_OR;
    }
    else if (buf[j] == '&' && buf[j + 1] == '&')
    {
        buf[j] = '\0';
        j++;
        info->command_buffer_type = CMD_AND;
    }
    else if (buf[j] == ';') /* Found end of this command */
    {
        buf[j] = '\0'; /* Replace semicolon with null */
        info->command_buffer_type = CMD_CHAIN;
    }
    else
        return (0);

    *p = j;
    return (1);
}

/**
 * check_chain_continue - checks if we should continue chaining based on the last status
 * @info: the parameter struct
 * @buf: the character buffer
 * @p: address of the current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain_continue(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (info->command_buffer_type == CMD_AND)
    {
        if (info->status)
        {
            buf[i] = '\0';
            j = len;
        }
    }
    if (info->command_buffer_type == CMD_OR)
    {
        if (!info->status)
        {
            buf[i] = '\0';
            j = len;
        }
    }

    *p = j;
}

/**
 * replace_alias - replaces an alias in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        p = _strchr(node->str, '=');
        if (!p)
            return (0);
        p = _strdup(p + 1);
        if (!p)
            return (0);
        info->argv[0] = p;
    }
    return (1);
}

/**
 * replace_vars - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?"))
        {
            replace_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->argv[i], "$$"))
        {
            replace_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->argv[i][1], '=');
        if (node)
        {
            replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->argv[i], _strdup(""));
    }
    return (0);
}

/**
 * replace_string - replaces a string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return (1);
}

