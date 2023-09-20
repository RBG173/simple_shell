#include "shell.h"

/**
 * fetch_environment - provides a copy of the environment strings as an array
 * @information: Structure containing potential arguments. Used to maintain
 *               constant function prototype.
 * Return: Always 0
 */
char **fetch_environment(information_t *information)
{
    if (!information->environment || information->env_changed)
    {
        information->environment = list_to_strings(information->environment_list);
        information->env_changed = 0;
    }

    return (information->environment);
}

/**
 * delete_environment_variable - Remove an environment variable
 * @information: Structure containing potential arguments. Used to maintain
 *               constant function prototype.
 * @variable: the string representing the environment variable
 * Return: 1 on deletion, 0 otherwise
 */
int delete_environment_variable(information_t *information, char *variable)
{
    list_t *node = information->environment_list;
    size_t index = 0;
    char *ptr;

    if (!node || !variable)
        return (0);

    while (node)
    {
        ptr = starts_with(node->str, variable);
        if (ptr && *ptr == '=')
        {
            information->env_changed = delete_node_at_index(&(information->environment_list), index);
            index = 0;
            node = information->environment_list;
            continue;
        }
        node = node->next;
        index++;
    }
    return (information->env_changed);
}

/**
 * set_environment_variable - Initialize a new environment variable,
 *                           or modify an existing one
 * @information: Structure containing potential arguments. Used to maintain
 *               constant function prototype.
 * @variable: the string representing the environment variable
 * @value: the string representing the environment variable value
 * Return: Always 0
 */
int set_environment_variable(information_t *information, char *variable, char *value)
{
    char *buffer = NULL;
    list_t *node;
    char *ptr;

    if (!variable || !value)
        return (0);

    buffer = malloc(_strlen(variable) + _strlen(value) + 2);
    if (!buffer)
        return (1);
    _strcpy(buffer, variable);
    _strcat(buffer, "=");
    _strcat(buffer, value);
    node = information->environment_list;
    while (node)
    {
        ptr = starts_with(node->str, variable);
        if (ptr && *ptr == '=')
        {
            free(node->str);
            node->str = buffer;
            information->env_changed = 1;
            return (0);
        }
        node = node->next;
    }
    add_node_end(&(information->environment_list), buffer, 0);
    free(buffer);
    information->env_changed = 1;
    return (0);
}

