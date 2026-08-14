#include "dynamic_tape.h"
#include "machine.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_TAPE_SIZE 1

static void resize_tape(struct tape *tape);

/**
 * @brief Create a tape struct
 *
 * @return struct tape*
 */
struct tape *create_tape()
{
    struct tape *tape = (struct tape *)malloc(sizeof(struct tape));
    if (tape == NULL)
    {
        printf("ERROR: Failed to allocate memory to tape\n");
        return tape;
    }
    tape->tape_size = INITIAL_TAPE_SIZE;
    tape->cells = (char *)malloc(sizeof(char) * tape->tape_size);
    // Initialise the tape to the blank symbol
    memset(tape->cells, BLANK_SYMBOL, sizeof(char) * tape->tape_size);
    return tape;
}

/**
 * @brief Doubles the size of the tape, writing the blank symbol to the newly added cells.
 *
 * @param tape
 * @return int
 */
static void resize_tape(struct tape *tape)
{
    int initial_tape_size = tape->tape_size;
    int new_tape_size = tape->tape_size * 2;
    char *new_cells = realloc(tape->cells, new_tape_size * sizeof(char));
    if (new_cells == NULL)
    {
        printf("ERROR: realloc failed while resizing tape\n");
        return;
    }
    tape->cells = new_cells;
    tape->tape_size = new_tape_size;

    // Write blank symbol to newly added tape cells.
    memset(tape->cells + initial_tape_size, BLANK_SYMBOL, sizeof(char) * initial_tape_size);
}

/**
 * @brief Write a char to the tape. If the head position is too big, resize the tape.
 *
 * @param tape tape to write to
 * @param head_position position on tape
 * @param c char to write
 */
void write_to_tape(struct tape *tape, int head_position, char c)
{
    if (head_position >= (tape->tape_size) - 1) //-1 so no matter what theres always 1 or more blank chars on the right of the tape
    {
        // printf("resizing tape!\n");
        resize_tape(tape);
    }
    tape->cells[head_position] = c;
}

/**
 * @brief Safely write null terminated string to start of tape. Resizes tape if needed.
 *
 * @param tape
 * @param string
 */
void populate_tape_with_string(struct tape *tape, char *string)
{
    char c;
    int i = 0;
    while ((c = string[i]) != '\0')
    {
        write_to_tape(tape, i, c);
        i++;
    }
}

/**
 * @brief Prints the tape, with a marker under the head position.
 *
 * @param tape
 * @param head_position
 */
void print_tape(struct tape *tape, int head_position)
{
    const char *prefix = "TAPE: ";
    printf("%s", prefix);
    for (int i = 0; i < tape->tape_size; i++)
    {
        putchar(tape->cells[i]);
    }
    putchar('\n');

    // marker line, aligned under the head position
    for (int i = 0; i < (int)strlen(prefix) + head_position; i++)
    {
        putchar(' ');
    }
    printf("^\n");
}