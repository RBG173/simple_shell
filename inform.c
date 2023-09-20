#include "shell.h"

/**
 * initialize_information - Initializes the information_t struct
 * @information: Address of the struct to be initialized
 */
void initialize_information(information_t *information)
{
    information->arguments = NULL;
    information->argument_vector = NULL;
    information->executable_path = NULL;
    information->argument_count = 0;
}

/**
 * populate_information - Populates the information_t struct
 * @information: Address of the struct to be populated
 * @argument_vector: Argument vector
 */
void populate_information(information_t *information, char **argument_vector)
{
    int i = 0;

    information->program_name = argument_vector[0];
    if (information->arguments)
    {
        information->argument_vector = tokenize_string(information->arguments, " \t");
        if (!information->argument_vector)
        {
            information->argument_vector = malloc(sizeof(char *) * 2);
            if (information->argument_vector)
            {
                information->argument_vector[0] = duplicate_string(information->arguments);
                information->argument_vector[1] = NULL;
            }
        }
        for (i = 0; information->argument_vector && information->argument_vector[i]; i++)
            ;
        information->argument_count = i;

        replace_aliases(information);
        replace_variables(information);
    }
}

/**
 * release_information - Frees the fields in the information_t struct
 * @information: Address of the struct to be released
 * @release_all: True if freeing all fields
 */
void release_information(information_t *information, int release_all)
{
    free_array(information->argument_vector);
    information->argument_vector = NULL;
    information->executable_path = NULL;
    if (release_all)
    {
        if (!information->command_buffer)
            free(information->arguments);
        if (information->environment)
            free_linked_list(&(information->environment));
        if (information->history)
            free_linked_list(&(information->history));
        if (information->alias_list)
            free_linked_list(&(information->alias_list));
        free_array(information->environment_strings);
        information->environment_strings = NULL;
        free_double_pointer((void **)information->command_buffer);
        if (information->read_file_descriptor > 2)
            close(information->read_file_descriptor);
        _putchar(BUFFER_FLUSH);
    }
}

