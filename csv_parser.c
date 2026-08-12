#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 64
#define MAX_ALPHABET_SIZE 16
#define MAX_NAME_LEN 32
#define DEBUG 0

// Load cell contents into buf and return which symbol terminated the read (',' or '\n' or EOF).
// If the field is too long, it will be truncated to fit in buf.
static int read_field(FILE *file, char *buf, int max_len)
{
    int i = 0;
    int c;
    while ((c = fgetc(file)) != EOF && c != ',' && c != '\n')
    {
        if (c == '\r' || c == '\t' || c == ' ')
        {
            continue; // Dont include certain whitespace chars
        }
        if (i < max_len - 1)
        {
            buf[i++] = (char)c;
        }
    }
    buf[i] = '\0';
    return c;
}

struct machine *load_machine_from_file(FILE *file)
{
    if (!file)
    {
        return NULL;
    }

    return NULL;
}