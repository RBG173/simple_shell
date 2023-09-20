#include "shell.h"

/**
 * shell_mode - checks if shell is in interactive mode
 * @shell_info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int shell_mode(info_t *shell_info)
{
	return (isatty(STDIN_FILENO) && shell_info->readfd <= 2);
}

/**
 * check_delim - verifies if character is a delimeter
 * @char_check: the char to verify
 * @delim_str: the delimeter string
 * Return: 1 if true, 0 if false
 */
int check_delim(char char_check, char *delim_str)
{
	while (*delim_str)
		if (*delim_str++ == char_check)
			return (1);
	return (0);
}

/**
 * check_alpha - verifies for alphabetic character
 * @char_input: The character to input
 * Return: 1 if char_input is alphabetic, 0 otherwise
 */

int check_alpha(int char_input)
{
	if ((char_input >= 'a' && char_input <= 'z') || (char_input >= 'A' && char_input <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * convert_to_int - converts a string to an integer
 * @str_input: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */

int convert_to_int(char *str_input)
{
	int index, sign_factor = 1, flag_status = 0, final_output;
	unsigned int result_val = 0;

	for (index = 0; str_input[index] != '\0' && flag_status != 2; index++)
	{
		if (str_input[index] == '-')
			sign_factor *= -1;

		if (str_input[index] >= '0' && str_input[index] <= '9')
		{
			flag_status = 1;
			result_val *= 10;
			result_val += (str_input[index] - '0');
		}
		else if (flag_status == 1)
			flag_status = 2;
	}

	if (sign_factor == -1)
		final_output = -result_val;
	else
		final_output = result_val;

	return (final_output);
}

