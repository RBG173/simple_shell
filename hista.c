#include "shell.h"

/**
 * obtain_history_file_path - retrieves the history file path
 * @information: structure holding parameters
 *
 * Return: allocated string containing the history file path
 */

char *obtain_history_file_path(information_t *information)
{
    char *path, *home_directory;

    home_directory = get_environment_variable(information, "HOME=");
    if (!home_directory)
        return (NULL);

    path = malloc(sizeof(char) * (_strlen(home_directory) + _strlen(HISTORY_FILE) + 2));
    if (!path)
        return (NULL);

    path[0] = '\0';
    _strcpy(path, home_directory);
    _strcat(path, "/");
    _strcat(path, HISTORY_FILE);

    return (path);
}

/**
 * record_history - creates a new file or appends to an existing file for history
 * @information: structure holding parameters
 *
 * Return: 1 on success, -1 on failure
 */
int record_history(information_t *information)
{
    ssize_t file_descriptor;
    char *file_path = obtain_history_file_path(information);
    list_t *node = NULL;

    if (!file_path)
        return (-1);

    file_descriptor = open(file_path, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(file_path);

    if (file_descriptor == -1)
        return (-1);

    for (node = information->history; node; node = node->next)
    {
        write(file_descriptor, node->str, _strlen(node->str));
        write(file_descriptor, "\n", 1);
    }

    close(file_descriptor);

    return (1);
}

/**
 * retrieve_history - reads and loads history from a file
 * @information: structure holding parameters
 *
 * Return: histcount on success, 0 on failure
 */
int retrieve_history(information_t *information)
{
    int i, last = 0, linecount = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stat;
    char *buffer = NULL, *file_path = obtain_history_file_path(information);

    if (!file_path)
        return (0);

    file_descriptor = open(file_path, O_RDONLY);
    free(file_path);

    if (file_descriptor == -1)
        return (0);

    if (!fstat(file_descriptor, &file_stat))
        file_size = file_stat.st_size;

    if (file_size < 2)
        return (0);

    buffer = malloc(sizeof(char) * (file_size + 1));

    if (!buffer)
        return (0);

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = '\0';

    if (read_length <= 0)
        return (free(buffer), 0);

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            add_history_entry(information, buffer + last, linecount++);
            last = i + 1;
        }
    }

    if (last != i)
        add_history_entry(information, buffer + last, linecount++);

    free(buffer);

    information->histcount = linecount;

    while (information->histcount-- >= HISTORY_MAX)
        delete_node_at_index(&(information->history), 0);

    renumber_history(information);

    return (information->histcount);
}

/**
 * add_history_entry - appends an entry to the history linked list
 * @information: structure holding parameters
 * @buffer: buffer
 * @linecount: the history line count (histcount)
 *
 * Return: Always 0
 */
int add_history_entry(information_t *information, char *buffer, int linecount)
{
    list_t *node = NULL;

    if (information->history)
        node = information->history;

    add_node_end(&node, buffer, linecount);

    if (!information->history)
        information->history = node;

    return (0);
}

/**
 * update_history_line_numbers - renumbers the history linked list after changes
 * @information: structure holding parameters
 *
 * Return: the new histcount
 */
int update_history_line_numbers(information_t *information)
{
    list_t *node = information->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return (information->histcount = i);
}

