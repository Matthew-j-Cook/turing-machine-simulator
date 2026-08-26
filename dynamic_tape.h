#ifndef TAPE_H
#define TAPE_H

struct tape
{
    int tape_size;
    char *cells;
};

// Create a tape struct
struct tape *create_tape();
// Write a char to the tape. If the head position is too big, resize the tape.
void write_to_tape(struct tape *tape, int head_position, char c);
// Safely write null terminated string to start of tape. Resizes tape if needed.
void populate_tape_with_string(struct tape *tape, char *string);
// Prints the tape, with a marker under the head position.
void print_tape(struct tape *tape, int head_position);

#endif // TAPE_H