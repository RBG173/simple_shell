#include "shell.h"

/**
 * data_buffer - buffers chained commands
 * @params: parameter structure
 * @buffer: address of buffer
 * @length: address of length variable
 *
 * Result: number of bytes read
 */
ssize_t data_buffer(params_t *params, char **buffer, size_t *length)
{
    ssize_t bytesRead = 0;
    size_t lengthPtr = 0;

    if (!*length) /* If the buffer is empty, fill it */
    {
        /*bfree((void **)params->cmd_buffer);*/
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, blockCtrlC);
#if USE_GETLINE
        bytesRead = getline(buffer, &lengthPtr, stdin);
#else
        bytesRead = _getline(params, buffer, &lengthPtr);
#endif
        if (bytesRead > 0)
        {
            if ((*buffer)[bytesRead - 1] == '\n')
            {
                (*buffer)[bytesRead - 1] = '\0'; /* Remove trailing newline */
                bytesRead--;
            }
            params->linecount_flag = 1;
            remove_comments(*buffer);
            build_history_list(params, *buffer, params->histcount++);
            /* if (_strchr(*buffer, ';')) Is this a command chain? */
            {
                *length = bytesRead;
                params->cmd_buffer = buffer;
            }
        }
    }
    return bytesRead;
}

/**
 * fetch_input - retrieves a line without the newline character
 * @params: parameter structure
 *
 * Result: number of bytes read
 */
ssize_t fetch_input(params_t *params)
{
    static char *buffer; /* The ';' command chain buffer */
    static size_t index, j, length;
    ssize_t bytesRead = 0;
    char **bufferPtr = &(params->arg), *ptr;

    _putchar(BUF_FLUSH);
    bytesRead = data_buffer(params, &buffer, &length);
    if (bytesRead == -1) /* EOF */
        return -1;
    if (length) /* Commands are left in the chain buffer */
    {
        j = index; /* Initialize a new iterator to the current buffer position */
        ptr = buffer + index; /* Get a pointer for return */

        check_chain(params, buffer, &j, index, length);
        while (j < length) /* Iterate to semicolon or the end */
        {
            if (is_chain(params, buffer, &j))
                break;
            j++;
        }

        index = j + 1; /* Increment past the nullified ';'' */
        if (index >= length) /* Reached the end of the buffer? */
        {
            index = length = 0; /* Reset the position and length */
            params->cmd_buffer_type = CMD_NORM;
        }

        *bufferPtr = ptr; /* Pass back a pointer to the current command position */
        return _strlen(ptr); /* Return the length of the current command */
    }

    *bufferPtr = buffer; /* Otherwise, not a chain, pass back the buffer from _getline() */
    return bytesRead; /* Return the length of the buffer from _getline() */
}

/**
 * read_buffer - reads a buffer
 * @params: parameter structure
 * @buffer: buffer
 * @index: size
 *
 * Result: bytesRead
 */
ssize_t read_buffer(params_t *params, char *buffer, size_t *index)
{
    ssize_t bytesRead = 0;

    if (*index)
        return 0;
    bytesRead = read(params->readfd, buffer, READ_BUFFER_SIZE);
    if (bytesRead >= 0)
        *index = bytesRead;
    return bytesRead;
}

/**
 * _getline_func - retrieves the next line of input from STDIN
 * @params: parameter structure
 * @ptr: address of a pointer to a buffer, preallocated or NULL
 * @length: size of the preallocated ptr buffer if not NULL
 *
 * Result: bytesRead
 */
int _getline_func(params_t *params, char **ptr, size_t *length)
{
    static char buffer[READ_BUFFER_SIZE];
    static size_t index, len;
    size_t k;
    ssize_t bytesRead = 0, size = 0;
    char *p = NULL, *newPtr = NULL, *c;

    p = *ptr;
    if (p && length)
        size = *length;
    if (index == len)
        index = len = 0;

    bytesRead = read_buffer(params, buffer, &len);
    if (bytesRead == -1 || (bytesRead == 0 && len == 0))
        return -1;

    c = _strchr(buffer + index, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : len;
    newPtr = _realloc(p, size, size ? size + k : k + 1);
    if (!newPtr) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (size)
        _strncat(newPtr, buffer + index, k - index);
    else
        _strncpy(newPtr, buffer + index, k - index + 1);

    size += k - index;
    index = k;
    p = newPtr;

    if (length)
        *length = size;
    *ptr = p;
    return size;
}

/**
 * blockCtrlC - prevents Ctrl-C from being executed
 * @signalNumber: the signal number
 *
 * Result: void
 */
void blockCtrlC(__attribute__((unused)) int signalNumber)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}

