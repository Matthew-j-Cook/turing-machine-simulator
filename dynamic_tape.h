#ifndef TAPE_H
#define TAPE_H

struct tape
{
    // int head_position;
    int tape_size;
    char *cells;
};

struct tape *create_tape();
void write_to_tape(struct tape *tape, int head_position, char c);
void populate_tape_with_string(struct tape *tape, char *string);
#endif // TAPE_H