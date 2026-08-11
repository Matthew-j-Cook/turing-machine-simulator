#include "dynamic_tape.h"
#include "machine.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_TAPE_SIZE 1

static void resize_tape(struct tape *tape);

struct tape *create_tape()
{
    struct tape *tape = (struct tape *)malloc(sizeof(struct tape));
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
    tape->tape_size *= 2;
    tape->cells = (char *)realloc(tape->cells, tape->tape_size * sizeof(char));
    // Write blank symbol to newly added tape cells.
    memset(tape->cells + initial_tape_size, BLANK_SYMBOL, sizeof(char) * initial_tape_size);
}

/**
 * @brief Write a char to the tape. If the head position is out of bounds, resize the tape.
 *
 * @param tape tape to write to
 * @param head_position position on tape
 * @param c char to write
 */
void write_to_tape(struct tape *tape, int head_position, char c)
{
    if (head_position >= (tape->tape_size) - 1) //-1 so no matter what theres always 1 or more blank chars on the right of the tape
    {
        printf("resizing tape!\n");
        resize_tape(tape);
    }
    tape->cells[head_position] = c;
}