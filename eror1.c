#include "shell.h"

/**
 * str_to_int - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in the string, the converted number otherwise
 *       -1 on error
 */
int str_to_int(char *str)
{
    int index = 0;
    unsigned long int result = 0;

    if (*str == '+')
        str++;  /* TODO: Why does this affect the main function's return value? */

    for (index = 0; str[index] != '\0'; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            result *= 10;
            result += (str[index] - '0');
            if (result > INT_MAX)
                return (-1);
        }
        else
            return (-1);
    }
    return (result);
}

/**
 * print_error_msg - prints an error message
 * @info: the parameter and return info struct
 * @errorMsg: string containing the specified error type
 * Return: None
 */
void print_error_msg(info_t *info, char *errorMsg)
{
    _eputs(info->fname);
    _eputs(": ");
    print_decimal(info->line_count, STDERR_FILENO);
    _eputs(": ");
    _eputs(info->argv[0]);
    _eputs(": ");
    _eputs(errorMsg);
}

/**
 * print_decimal - function prints a decimal (integer) number (base 10)
 * @value: the input value
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int print_decimal(int value, int fd)
{
    int (*output_char)(char) = _putchar;
    int i, count = 0;
    unsigned int absolute, current;

    if (fd == STDERR_FILENO)
        output_char = _eputchar;
    
    if (value < 0)
    {
        absolute = -value;
        output_char('-');
        count++;
    }
    else
        absolute = value;

    current = absolute;
    
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (absolute / i)
        {
            output_char('0' + current / i);
            count++;
        }
        current %= i;
    }
    output_char('0' + current);
    count++;

    return (count);
}

/**
 * convert_to_string - converter function, a clone of itoa
 * @number: number to convert
 * @base: base for conversion
 * @flags: argument flags
 *
 * Return: string representation of the number
 */
char *convert_to_string(long int number, int base, int flags)
{
    static char *hexDigits;
    static char buffer[50];
    char sign = 0;
    char *result;
    unsigned long n = number;

    if (!(flags & CONVERT_UNSIGNED) && number < 0)
    {
        n = -number;
        sign = '-';
    }

    hexDigits = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
    result = &buffer[49];
    *result = '\0';

    do
    {
        *--result = hexDigits[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--result = sign;
    
    return (result);
}

/**
 * remove_first_comment - function replaces the first instance of '#' with '\0'
 * @text: address of the string to modify
 *
 * Return: Always 0;
 */
void remove_first_comment(char *text)
{
    int i;

    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '#' && (!i || text[i - 1] == ' '))
        {
            text[i] = '\0';
            break;
        }
    }
}

