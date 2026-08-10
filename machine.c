#include <stdio.h>
#include "machine.h"

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
        return;
    }
    state->edges[state->num_edges] = edge;
    state->num_edges++;
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
}
struct machine *create_machine(struct state *initial_state, struct state **states, int num_states,
                               int head_position, int *tape, int tape_length)
{
    struct machine *newMachine = (struct machine *)malloc(sizeof(struct machine));
    newMachine->initial_state = initial_state;
    newMachine->states = states;
    newMachine->num_states = num_states;
    newMachine->head_position = head_position;
    newMachine->tape = tape;
    newMachine->tape_length = tape_length;
    return newMachine;
}