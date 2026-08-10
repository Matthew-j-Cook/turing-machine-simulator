struct state;
enum action_type
{
    WRITE,
    MOVE_LEFT,
    MOVE_RIGHT
};

struct transition
{
    struct state *to_state;  // The state to which the transition leads
    char required_symbol;    // The symbol that triggers the transition
    enum action_type action; // The action to perform (write or move)
    char write_symbol;       // Determines what the head will do, write or move.
};

struct state
{
    int id;                         // The unique identifier for the state
    struct transition *transitions; // Pointer to an array of transitions for this state
    int num_transitions;            // The number of transitions for this state
    char is_accepting;              // Flag indicating if this state is an accepting state
};
