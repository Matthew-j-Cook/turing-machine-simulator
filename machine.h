#ifndef MACHINE_H
#define MACHINE_H

#include "dynamic_tape.h"
#include "state.h"

struct machine
{
    struct state *current_state; // Pointer to the current state, starts at initial state.
    int num_states;              // The number of states in the machine
    int head_position;           // The current position of the tape head
    struct tape *tape;
};

struct machine *create_machine(struct state *initial_state, int head_position, struct tape *tape);

int is_computation_complete(struct machine *machine);
int step_machine(struct machine *machine);
void print_machine_state(struct machine *machine);
#endif // MACHINE_H