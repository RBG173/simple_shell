#include "shell.h"

/**
 * shell_exit - terminates the shell
 * @shell_info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: terminates with a given exit status
 * (0) if shell_info.argv[0] != "exit"
 */
int shell_exit(info_t *shell_info)
{
	int exit_status_check;

	if (shell_info->argv[1]) /* If there is an exit argument */
	{
		exit_status_check = _erratoi(shell_info->argv[1]);
		if (exit_status_check == -1)
		{
			shell_info->status = 2;
			print_error(shell_info, "Illegal number: ");
			_eputs(shell_info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		shell_info->err_num = _erratoi(shell_info->argv[1]);
		return (-2);
	}
	shell_info->err_num = -1;
	return (-2);
}

/**
 * shell_cd - changes the current directory of the process
 * @shell_info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int shell_cd(info_t *shell_info)
{
	char *current_dir, *new_dir, dir_buffer[1024];
	int change_dir_ret;

	current_dir = getcwd(dir_buffer, 1024);
	if (!current_dir)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!shell_info->argv[1])
	{
		new_dir = _getenv(shell_info, "HOME=");
		if (!new_dir)
			change_dir_ret = /* TODO: what should this be? */
				chdir((new_dir = _getenv(shell_info, "PWD=")) ? new_dir : "/");
		else
			change_dir_ret = chdir(new_dir);
	}
	else if (_strcmp(shell_info->argv[1], "-") == 0)
	{
		if (!_getenv(shell_info, "OLDPWD="))
		{
			_puts(current_dir);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(shell_info, "OLDPWD=")), _putchar('\n');
		change_dir_ret = /* TODO: what should this be? */
			chdir((new_dir = _getenv(shell_info, "OLDPWD=")) ? new_dir : "/");
	}
	else
		change_dir_ret = chdir(shell_info->argv[1]);
	if (change_dir_ret == -1)
	{
		print_error(shell_info, "can't cd to ");
		_eputs(shell_info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(shell_info, "OLDPWD", _getenv(shell_info, "PWD="));
		_setenv(shell_info, "PWD", getcwd(dir_buffer, 1024));
	}
	return (0);
}

/**
 * shell_help - provides help for the shell commands
 * @shell_info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int shell_help(info_t *shell_info)
{
	char **args_array;

	args_array = shell_info->argv;
    _puts("Help function call works. Function not yet implemented \n");
    if (0)
        _puts(*args_array); /* temp att_unused workaround */
    return (0);
}

