#ifndef MACHINE_H
#define MACHINE_H

#define BLANK_SYMBOL '_'
#define MOVE_RIGHT_SYMBOL 'R'
#define MOVE_LEFT_SYMBOL 'L'
#define MAX_EDGES_PER_STATE 10

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
    int id;                 // The unique identifier for the state
    struct edge *edges[10]; // Pointer to an array of edges for this state
    int num_edges;          // The number of transition edges for this state
    char is_accepting;      // Flag indicating if this state is an accepting state
};

struct machine
{
    struct state *initial_state; // Pointer to the initial state of the machine
    struct state **states;       // Pointer to an array of all states in the machine
    int num_states;              // The number of states in the machine
    int head_position;           // The current position of the tape head
    char *tape;                  // Pointer to the tape (array of symbols)
    int tape_length;             // The length of the tape
};

struct state *create_state(int id, char is_accepting);
void delete_state(struct state *state);

int add_edge_to_state(struct state *state, struct edge *edge);

struct edge *create_edge(struct state *to_state, char required_symbol,
                         enum action_type action, char write_symbol);
struct machine *create_machine(struct state *initial_state, struct state **states, int num_states,
                               int head_position, int *tape, int tape_length);
#endif // MACHINE_H