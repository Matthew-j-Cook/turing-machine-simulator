#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "state.h"
/**
 * @brief Create a state object
 *
 * @param name The name of the state, should be unique
 * @param is_accepting
 * @return struct state*
 */
struct state *create_state(char *name, char is_accepting)
{
    struct state *newState = (struct state *)malloc(sizeof(struct state));
    if (newState == NULL)
    {
        printf("ERROR: Failed to allocate memory to state\n");
        return newState;
    }
    strncpy(newState->name, name, 32 - 1);
    newState->num_edges = 0;
    newState->is_accepting = is_accepting == 1 ? 1 : 0; // Set to 1 or 0
    return newState;
}

/**
 * @brief Deletes a state and its edges.
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
 * @brief Adds an edge to a state, ensures the number of edges does not exceed the max allowed.
 *
 * @param state
 * @param edge
 * @return int 0 on success, 1 if the state has too many edges
 */
int add_edge_to_state(struct state *state, struct edge *edge)
{
    if (edge == NULL)
    {
        return 1;
    }

    // Reject duplicate transitions for the same input symbol
    for (int i = 0; i < state->num_edges; i++)
    {
        if (state->edges[i]->required_symbol == edge->required_symbol)
        {
            printf("ERROR: duplicate transition in state %s for symbol '%c'\n",
                   state->name, edge->required_symbol);
            free(edge);
            return 1;
        }
    }

    if (state->num_edges >= MAX_EDGES_PER_STATE)
    {
        printf("WARNING: Too many edges for state: %s", state->name);
        return 1;
    }
    state->edges[state->num_edges] = edge;
    state->num_edges++;
    return 0;
}

/**
 * @brief Create an edge object
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
    // Disallow using the move symbols as write symbols.
    if (action == WRITE)
    {
        if (write_symbol == MOVE_LEFT_SYMBOL || write_symbol == MOVE_RIGHT_SYMBOL)
        {
            printf("ERROR: Cannot use move left and right symbols for the write symbol");
            return NULL;
        }
    }
    struct edge *edge = (struct edge *)malloc(sizeof(struct edge));
    if (edge == NULL)
    {
        printf("ERROR: Failed to allocate memory to edge\n");
        return edge;
    }
    edge->to_state = to_state;
    edge->required_symbol = required_symbol;
    edge->action = action;
    edge->write_symbol = write_symbol;
    return edge;
}

/**
 * @brief Given a symbol y ⊂ Y and a state q ⊂ Q, find which edge should be traversed
 *
 * @param symbol
 * @param state
 * @return struct edge*
 */
struct edge *find_next_edge(int symbol, struct state *state)
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