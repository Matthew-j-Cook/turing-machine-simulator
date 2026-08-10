#include <stdio.h>
#include "machine.h"

int main()
{
    struct state *q0 = create_state(0, 0);
    struct state *q1 = create_state(1, 0);
    struct state *q2 = create_state(2, 0);
    struct state *q3 = create_state(3, 0);
    struct state *q4 = create_state(4, 0);
    struct state *q5 = create_state(5, 1);

    struct edge *q0e0 = create_edge(q1, '1', WRITE, BLANK_SYMBOL);
    struct edge *q0e1 = create_edge(q1, '0', WRITE, BLANK_SYMBOL);
    add_edge_to_state(q0, q0e0);
    add_edge_to_state(q0, q0e1);

    struct edge *q1e0 = create_edge(q2, BLANK_SYMBOL, MOVE_RIGHT, -1);
    add_edge_to_state(q1, q1e0);

    struct edge *q2e0 = create_edge(q3, '1', WRITE, '1');
    struct edge *q2e1 = create_edge(q3, '0', WRITE, '1');
    add_edge_to_state(q2, q2e0);
    add_edge_to_state(q2, q2e1);

    // struct machine machine = create_machine();
    return 0;
}