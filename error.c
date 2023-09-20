#include "shell.h"

/**
 * print_str - prints an input string
 * @input_str: the string to be printed
 *
 * Return: Nothing
 */
void print_str(char *input_str)
{
	int index = 0;

	if (!input_str)
		return;
	while (input_str[index] != '\0')
	{
		print_char(input_str[index]);
		index++;
	}
}

/**
 * print_char - writes the character to stderr
 * @char_to_print: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int print_char(char char_to_print)
{
	static int buffer_index;
	static char buffer[WRITE_BUF_SIZE];

	if (char_to_print == BUF_FLUSH || buffer_index >= WRITE_BUF_SIZE)
	{
		write(2, buffer, buffer_index);
		buffer_index = 0;
	}
	if (char_to_print != BUF_FLUSH)
		buffer[buffer_index++] = char_to_print;
	return (1);
}

/**
 * write_char_to_fd - writes the character to given fd
 * @char_to_write: The character to print
 * @file_desc: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_char_to_fd(char char_to_write, int file_desc)
{
	static int buffer_index;
	static char buffer[WRITE_BUF_SIZE];

	if (char_to_write == BUF_FLUSH || buffer_index >= WRITE_BUF_SIZE)
	{
		write(file_desc, buffer, buffer_index);
		buffer_index = 0;
	}
	if (char_to_write != BUF_FLUSH)
		buffer[buffer_index++] = char_to_write;
	return (1);
}

/**
 * print_str_to_fd - prints an input string
 * @input_str: the string to be printed
 * @file_desc: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int print_str_to_fd(char *input_str, int file_desc)
{
	int index = 0;

	if (!input_str)
		return (0);
	while (*input_str)
	{
		index += write_char_to_fd(*input_str++, file_desc);
	}
	return (index);
}

