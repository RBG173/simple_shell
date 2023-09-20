#include "shell.h"

/**
 * shell_history - displays the command history list, one command per line, preceded
 *                 with line numbers, starting at 0.
 * @shell_info: Structure containing potential arguments. Used to maintain
 *              constant function prototype.
 *  Return: Always 0
 */
int shell_history(info_t *shell_info)
{
	print_list(shell_info->history);
	return (0);
}

/**
 * remove_alias - removes alias from string
 * @shell_info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int remove_alias(info_t *shell_info, char *alias_str)
{
	char *alias_ptr, alias_char;
	int ret_val;

	alias_ptr = _strchr(alias_str, '=');
	if (!alias_ptr)
		return (1);
	alias_char = *alias_ptr;
	*alias_ptr = 0;
	ret_val = delete_node_at_index(&(shell_info->alias),
		get_node_index(shell_info->alias, node_starts_with(shell_info->alias, alias_str, -1)));
	*alias_ptr = alias_char;
	return (ret_val);
}

/**
 * assign_alias - assigns alias to string
 * @shell_info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int assign_alias(info_t *shell_info, char *alias_str)
{
	char *alias_ptr;

	alias_ptr = _strchr(alias_str, '=');
	if (!alias_ptr)
		return (1);
	if (!*++alias_ptr)
		return (remove_alias(shell_info, alias_str));

	remove_alias(shell_info, alias_str);
	return (add_node_end(&(shell_info->alias), alias_str, 0) == NULL);
}

/**
 * display_alias - displays an alias string
 * @node_alias: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int display_alias(list_t *node_alias)
{
	char *p = NULL, *a = NULL;

	if (node_alias)
	{
		p = _strchr(node_alias->str, '=');
		for (a = node_alias->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * shell_alias - mimics the alias builtin (man alias)
 * @shell_info: Structure containing potential arguments. Used to maintain
 *              constant function prototype.
 *  Return: Always 0
 */
int shell_alias(info_t *shell_info)
{
	int i = 0;
	char *p = NULL;
	list_t *node_alias = NULL;

	if (shell_info->argc == 1)
	{
		node_alias = shell_info->alias;
		while (node_alias)
		{
			display_alias(node_alias);
			node_alias = node_alias->next;
		}
		return (0);
	}
	for (i = 1; shell_info->argv[i]; i++)
	{
		p = _strchr(shell_info->argv[i], '=');
		if (p)
			assign_alias(shell_info, shell_info->argv[i]);
		else
			display_alias(node_starts_with(shell_info->alias, shell_info->argv[i], '='));
	}

	return (0);
}

