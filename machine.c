#include <stdio.h>
#include <stdlib.h>
#include "machine.h"

// Conventions:
// Let X be the input alphabet
// Let Y be the tape alphabet

struct state *create_state(int id, char is_accepting)
{
    struct state *newState = (struct state *)malloc(sizeof(struct state));
    newState->id = id;
    newState->num_edges = 0;
    newState->is_accepting = is_accepting == 1 ? 1 : 0; // Set to 1 or 0
    return newState;
}

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
void delete_state(struct state *state)
{
    // Free edges
    for (int i = 0; i < state->num_edges; i++)
    {
        free(state->edges[i]);
    }
    free(state);
}
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
struct machine *create_machine(struct state *initial_state, int head_position,
                               char *tape, int tape_length)
{
    struct machine *newMachine = (struct machine *)malloc(sizeof(struct machine));
    newMachine->current_state = initial_state;
    newMachine->head_position = head_position;
    newMachine->tape = tape;
    newMachine->tape_length = tape_length;
    return newMachine;
}

/**
 * @brief
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
    // Check Y ⊂ X
    return 1;
}

int step_machine(struct machine *machine)
{
    struct edge *next_edge = find_next_edge_from_symbol((machine->tape)[machine->head_position],
                                                        machine->current_state);
    if (next_edge == NULL)
    {
        printf("FATAL ERROR\n");
        return -1;
    }
    // printf("NEXT EDGE IS: %c\n", next_edge->required_symbol);

    if (next_edge->action == WRITE)
    {
        (machine->tape)[machine->head_position] = next_edge->write_symbol;
    }
    if (next_edge->action == MOVE_LEFT)
    {
        machine->head_position--;
    }
    if (next_edge->action == MOVE_RIGHT)
    {
        machine->head_position++;
    }
    machine->current_state = next_edge->to_state;

    return 0;
}

/**
 * @brief Given a symbol and a state, find which edge should be traversed next
 *
 * @param symbol
 * @param state
 * @return struct edge*
 */
struct edge *find_next_edge_from_symbol(int symbol, struct state *state)
{
    for (int i = 0; i < state->num_edges; i++)
    {
        struct edge *current_edge = (struct edge *)state->edges[i];
        if (current_edge->required_symbol == symbol)
        {
            return current_edge;
        }
    }
    printf("STATE %d is pointing at %d EDGES", state->id, state->num_edges);
    return NULL;
}