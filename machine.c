#include <stdio.h>
#include <stdlib.h>
#include "machine.h"

// Conventions:
// Let X be the input alphabet
// Let Y be the tape alphabet
// Let Q be the set of states

/**
 * @brief Create a state struct
 *
 * @param id
 * @param is_accepting
 * @return struct state*
 */
struct state *create_state(int id, char is_accepting)
{
    struct state *newState = (struct state *)malloc(sizeof(struct state));
    newState->id = id;
    newState->num_edges = 0;
    newState->is_accepting = is_accepting == 1 ? 1 : 0; // Set to 1 or 0
    return newState;
}

/**
 * @brief Adds an edge to a state, ensures the number of edges does not exceed the max allowed.
 *
 * @param state
 * @param edge
 * @return int 0 on success, 1 if the state has too many edges
 */
int add_edge_to_state(struct state *state, struct edge *edge)
{
    if (state->num_edges > MAX_EDGES_PER_STATE)
    {
        printf("WARNING: Too many edges for state of id: %d", state->id);
        return 1;
    }
    state->edges[state->num_edges] = edge;
    state->num_edges++;
    return 0;
}
/**
 * @brief Deletes a state and frees its associated memory.
 *
 * @param state
 */
void delete_state(struct state *state)
{
    // Free edges
    for (int i = 0; i < state->num_edges; i++)
    {
        free(state->edges[i]);
    }
    free(state);
}
/**
 * @brief Create a edge object
 *
 * @param to_state The state the edge points to
 * @param required_symbol The symbol read on the tape that triggers this edge
 * @param action The action to perform when this edge is taken(write or move)
 * @param write_symbol The symbol to write to the tape if the action is WRITE, ignored otherwise.
 * @return struct edge*
 */
struct edge *create_edge(struct state *to_state, char required_symbol,
                         enum action_type action, char write_symbol)
{
    if (write_symbol == MOVE_LEFT_SYMBOL | write_symbol == MOVE_RIGHT_SYMBOL)
    {
        printf("WARNING: INVALID EDGE SYMBOL: %c", required_symbol);
        return NULL;
    }
    struct edge *edge = (struct edge *)malloc(sizeof(struct edge));
    edge->to_state = to_state;
    edge->required_symbol = required_symbol;
    edge->action = action;
    edge->write_symbol = write_symbol;
    return edge;
}

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
    newMachine->current_state = initial_state;
    newMachine->head_position = head_position;
    newMachine->tape = tape;
    return newMachine;
}

/**
 * @brief Performs a single step of the Turing machine.
 * @details Takes the current state of the machine and determines the next action based on the head's symbol and transition rules.
 *
 * @param machine
 * @return int
 */
int step_machine(struct machine *machine)
{
    struct edge *next_edge = find_matching_edge((machine->tape->cells)[machine->head_position],
                                                machine->current_state);

    // If no matching edge is found, the computation is aborted.
    if (next_edge == NULL)
    {
        return -1;
    }
    // Perform the action associated with the edge
    switch (next_edge->action)
    {
    case WRITE:
        // Write to tape, resize of head position is larger than tape.
        write_to_tape(machine->tape, machine->head_position, next_edge->write_symbol);
        if (machine->tape == NULL)
        {
            printf("realloc could not find enough space on heap!!\n");
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

/**
 * @brief Given a symbol y ⊂ Y and a state q ⊂ Q, find which edge should be traversed
 *
 * @param symbol
 * @param state
 * @return struct edge*
 */
struct edge *find_matching_edge(int symbol, struct state *state)
{
    // Find which edge the current state has
    for (int i = 0; i < state->num_edges; i++)
    {
        struct edge *current_edge = state->edges[i];
        if (current_edge->required_symbol == symbol)
        {
            return current_edge;
        }
    }
    return NULL;
}