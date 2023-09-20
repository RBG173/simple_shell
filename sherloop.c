#include "shell.h"

/**
 * shell_loop - Main shell loop
 * @info: The parameter and return info struct
 * @av: The argument vector from main()
 *
 * Return: 0 on success, 1 on error, or an error code
 */
int shell_loop(info_t *info, char **av)
{
    ssize_t read_result = 0;
    int builtin_ret = 0;

    while (read_result != -1 && builtin_ret != -2)
    {
        clear_info(info);
        if (is_interactive(info))
            print_prompt("$ ");
        flush_output_buffer();
        read_result = get_user_input(info);
        
        if (read_result != -1)
        {
            set_info_from_args(info, av);
            builtin_ret = find_and_execute_builtin(info);
            if (builtin_ret == -1)
                find_and_execute_command(info);
        }
        else if (is_interactive(info))
            print_newline();
        
        free_info_struct(info, 0);
    }

    write_history_to_file(info);
    free_info_struct(info, 1);

    if (!is_interactive(info) && info->status)
        exit(info->status);
    
    if (builtin_ret == -2)
    {
        if (info->error_code == -1)
            exit(info->status);
        exit(info->error_code);
    }
    
    return (builtin_ret);
}

/**
 * find_and_execute_builtin - Finds and executes a builtin command
 * @info: The parameter and return info struct
 *
 * Return: -1 if builtin not found,
 *          0 if builtin executed successfully,
 *          1 if builtin found but not successful,
 *          2 if builtin signals exit()
 */
int find_and_execute_builtin(info_t *info)
{
    int i, builtin_ret = -1;
    builtin_info builtins[] = {
        {"exit", execute_exit_builtin},
        {"env", execute_env_builtin},
        {"help", execute_help_builtin},
        {"history", execute_history_builtin},
        {"setenv", execute_setenv_builtin},
        {"unsetenv", execute_unsetenv_builtin},
        {"cd", execute_cd_builtin},
        {"alias", execute_alias_builtin},
        {NULL, NULL}
    };

    for (i = 0; builtins[i].name; i++)
    {
        if (are_strings_equal(info->argv[0], builtins[i].name))
        {
            info->line_count++;
            builtin_ret = builtins[i].function(info);
            break;
        }
    }

    return (builtin_ret);
}

/**
 * find_and_execute_command - Finds and executes a command in the PATH
 * @info: The parameter and return info struct
 *
 * Return: void
 */
void find_and_execute_command(info_t *info)
{
    char *path = NULL;
    int i, non_whitespace_count;

    info->path = info->argv[0];

    if (info->line_count_flag == 1)
    {
        info->line_count++;
        info->line_count_flag = 0;
    }

    non_whitespace_count = 0;
    
    for (i = 0; info->arg[i]; i++)
    {
        if (!is_delimiter(info->arg[i], " \t\n"))
            non_whitespace_count++;
    }

    if (non_whitespace_count == 0)
        return;

    path = find_command_in_path(info, get_environment_variable(info, "PATH="), info->argv[0]);

    if (path)
    {
        info->path = path;
        fork_and_execute_command(info);
    }
    else
    {
        if ((is_interactive(info) || get_environment_variable(info, "PATH=") || info->argv[0][0] == '/') && is_command(info, info->argv[0]))
            fork_and_execute_command(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            print_error_message(info, "not found\n");
        }
    }
}

/**
 * fork_and_execute_command - Forks and executes a command
 * @info: The parameter and return info struct
 *
 * Return: void
 */
void fork_and_execute_command(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    
    if (child_pid == 0)
    {
        if (execute_command(info->path, info->argv, get_environment_variables(info)) == -1)
        {
            free_info_struct(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (child_process_exited_normally(info->status))
        {
            info->status = get_exit_status(info->status);
            if (info->status == 126)
                print_error_message(info, "Permission denied\n");
        }
    }
}

