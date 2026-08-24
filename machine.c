#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "state.h"
// Conventions:
// Let X be the input alphabet
// Let Y be the tape alphabet
// Let Q be the set of states

/**
 * @brief Create a machine object
 *
 * @param initial_state The initial state of the machine
 * @param head_position The initial head position, should be 0 most of the time.
 * @param tape The tape content
 * @param tape_length The length of the tape
 * @return struct machine*
 */
struct machine *create_machine(struct state *initial_state, int head_position,
                               struct tape *tape)
{
    struct machine *newMachine = (struct machine *)malloc(sizeof(struct machine));
    if (newMachine == NULL)
    {
        printf("ERROR: Failed to allocate memory to machine\n");
        return newMachine;
    }
    newMachine->current_state = initial_state;
    newMachine->head_position = head_position;
    newMachine->tape = tape;
    return newMachine;
}

/**
 * @brief Performs a single step of the Turing machine.
 * @details Takes the current state of the machine and determines the next
 * action based on the head's symbol and transition rules.
 *
 * @param machine
 * @return int
 */
int step_machine(struct machine *machine)
{
    struct edge *next_edge = find_next_edge((machine->tape->cells)[machine->head_position],
                                            machine->current_state);

    // Undefined transition for state symbol pair. the computation is aborted.
    if (next_edge == NULL)
    {
        printf("Undefined transition: No transition was found for state id: %s and symbol: \"%c\" ", machine->current_state->name, machine->tape->cells[machine->head_position]);
        return -1;
    }
    // Perform the action associated with the edge
    switch (next_edge->action)
    {
    case WRITE:
        // Write to tape. Will resize if head position is close to tape size.
        write_to_tape(machine->tape, machine->head_position, next_edge->write_symbol);
        if (machine->tape == NULL)
        {
            printf("Realloc could not find enough space on heap!!\n");
        }
        break;
    case MOVE_LEFT:
        if (machine->head_position == 0)
        {
            printf("WARNING: Attempted to move left from the leftmost position.\n");
            return -1;
        }
        machine->head_position--;
        break;
    case MOVE_RIGHT:

        machine->head_position++;
        break;
    }
    // Advance to the next state
    machine->current_state = next_edge->to_state;

    return 0;
}

/**
 * @brief Checks if the Turing machine has completed its computation.
 * @details Considered complete if:
 * 1. The head is parked at the leftmost position (head_position == 0)
 * 2. The current state is an accepting state (is_accepting == 1)
 * 3. The tape content is a subset of the input alphabet (Y ⊂ X)
 *
 * @param machine
 * @return int 1 if complete, 0 if not
 */
int is_computation_complete(struct machine *machine)
{
    // Head not parked
    if (!(machine->head_position == 0))
    {
        return 0;
    }
    // Not in an accepting state
    if (machine->current_state->is_accepting != 1)
    {
        return 0;
    }
    // TODO: Check Y ⊂ X
    return 1;
}

void print_machine_state(struct machine *machine)
{
    printf("STATE: %s\n", machine->current_state->name);
    print_tape(machine->tape, machine->head_position);
}
