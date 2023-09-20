#include "shell.h"

/**
 **_copyStr - duplicates a string
 *@target: the target string to be copied to
 *@source: the source string
 *@num: the number of characters to be copied
 *Return: the duplicated string
 */
char *_copyStr(char *target, char *source, int num)
{
	int idx, jdx;
	char *str = target;

	idx = 0;
	while (source[idx] != '\0' && idx < num - 1)
	{
		target[idx] = source[idx];
		idx++;
	}
	if (idx < num)
	{
		jdx = idx;
		while (jdx < num)
		{
			target[jdx] = '\0';
			jdx++;
		}
	}
	return (str);
}

/**
 **_concatStr - joins two strings
 *@target: the first string
 *@source: the second string
 *@num: the maximum number of bytes to be used
 *Return: the joined string
 */
char *_concatStr(char *target, char *source, int num)
{
	int idx, jdx;
	char *str = target;

	idx = 0;
	jdx = 0;
	while (target[idx] != '\0')
		idx++;
	while (source[jdx] != '\0' && jdx < num)
	{
		target[idx] = source[jdx];
		idx++;
		jdx++;
	}
	if (jdx < num)
		target[idx] = '\0';
	return (str);
}

/**
 **_findChar - finds a character in a string
 *@str: the string to be searched
 *@ch: the character to search for
 *Return: (str) a pointer to the memory area str
 */
char *_findChar(char *str, char ch)
{
	do {
		if (*str == ch)
			return (str);
	} while (*str++ != '\0');

	return (NULL);
}

