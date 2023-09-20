#include "shell.h"

/**
 * linked_list_length - determines the length of a linked list
 * @head: pointer to the first node
 *
 * Return: size of the list
 */
size_t linked_list_length(const list_t *head)
{
    size_t length = 0;

    while (head)
    {
        head = head->next;
        length++;
    }

    return length;
}

/**
 * linked_list_to_strings - returns an array of strings from the list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **linked_list_to_strings(list_t *head)
{
    list_t *node = head;
    size_t num_elements = linked_list_length(head);
    char **strings;
    char *str;

    if (!head || !num_elements)
        return NULL;

    strings = malloc(sizeof(char *) * (num_elements + 1));
    if (!strings)
        return NULL;

    for (size_t i = 0; node; node = node->next, i++)
    {
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            for (size_t j = 0; j < i; j++)
                free(strings[j]);
            free(strings);
            return NULL;
        }

        str = _strcpy(str, node->str);
        strings[i] = str;
    }

    strings[num_elements] = NULL;
    return strings;
}

/**
 * print_linked_list - prints all elements of a list_t linked list
 * @head: pointer to the first node
 *
 * Return: size of list
 */
size_t print_linked_list(const list_t *head)
{
    size_t i = 0;

    while (head)
    {
        _puts(convert_number(head->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(head->str ? head->str : "(nil)");
        _puts("\n");
        head = head->next;
        i++;
    }

    return i;
}

/**
 * find_node_with_prefix - returns the node whose string starts with a prefix
 * @node: pointer to the list head
 * @prefix: string to match
 * @next_char: the next character after the prefix to match
 *
 * Return: matching node or NULL
 */
list_t *find_node_with_prefix(list_t *node, char *prefix, char next_char)
{
    char *p = NULL;

    while (node)
    {
        p = starts_with(node->str, prefix);
        if (p && ((next_char == -1) || (*p == next_char)))
            return node;
        node = node->next;
    }

    return NULL;
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to the list head
 * @node: pointer to the node
 *
 * Return: index of the node or -1
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
    size_t index = 0;

    while (head)
    {
        if (head == node)
            return index;
        head = head->next;
        index++;
    }

    return -1;
}

=========================

main.c code

#include "shell.h"

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
    info_t info[] = { INFO_INIT };
    int file_descriptor = 2;

    asm ("mov %1, %0\n\t"
         "add $3, %0"
         : "=r" (file_descriptor)
         : "r" (file_descriptor));

    if (argc == 2)
    {
        file_descriptor = open(argv[1], O_RDONLY);
        if (file_descriptor == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                _eputs(argv[0]);
                _eputs(": 0: Can't open ");
                _eputs(argv[1]);
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        info->readfd = file_descriptor;
    }

    populate_environment_list(info);
    read_history(info);
    hsh(info, argv);

    return (EXIT_SUCCESS);
}

