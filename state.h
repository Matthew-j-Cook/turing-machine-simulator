#ifndef STATE_H
#define STATE_H

#define MAX_EDGES_PER_STATE 10
#define MAX_NAME_LENGTH 32
#define BLANK_SYMBOL '_'
#define MOVE_RIGHT_SYMBOL 'R'
#define MOVE_LEFT_SYMBOL 'L'
struct state;
enum action_type
{
    WRITE,
    MOVE_LEFT,
    MOVE_RIGHT
};

struct edge
{
    struct state *to_state;  // The state to which the transition leads
    char required_symbol;    // The symbol that triggers the transition
    enum action_type action; // The action to perform (write or move)
    char write_symbol;       // Determines what the head will do, write or move.
};

struct state
{
    char name[MAX_NAME_LENGTH];              // The unique identifier for the state
    struct edge *edges[MAX_EDGES_PER_STATE]; // Pointer to an array of edges for this state
    int num_edges;                           // The number of transition edges for this state
    char is_accepting;                       // Flag indicating if this state is an accepting state
};

struct state *create_state(char *name, char is_accepting);
void delete_state(struct state *state);

struct edge *create_edge(struct state *to_state, char required_symbol,
                         enum action_type action, char write_symbol);
int add_edge_to_state(struct state *state, struct edge *edge);
struct edge *find_next_edge(int symbol, struct state *state);

#endif // STATE_H