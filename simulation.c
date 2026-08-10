#include <stdio.h>
#include "machine.h"
#include <string.h>
#include <unistd.h>
void print_tape(struct machine *machine);
void print_tape(struct machine *machine)
{
    const char *prefix = "TAPE: ";
    printf("%s", prefix);
    for (int i = 0; i < machine->tape_length; i++)
    {
        putchar(machine->tape[i]);
    }
    putchar('\n');

    // marker line, aligned under the head position
    for (int i = 0; i < (int)strlen(prefix) + machine->head_position; i++)
    {
        putchar(' ');
    }
    printf("^\n");
}

int main()
{
    char tape[1024] = "10101010101____";
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

    struct edge *q2e0 = create_edge(q3, '1', MOVE_RIGHT, -1);
    struct edge *q2e1 = create_edge(q3, '0', MOVE_RIGHT, -1);
    add_edge_to_state(q2, q2e0);
    add_edge_to_state(q2, q2e1);

    struct edge *q3e0 = create_edge(q2, '0', WRITE, '1');
    struct edge *q3e1 = create_edge(q2, '1', WRITE, '1');
    struct edge *q3e2 = create_edge(q4, BLANK_SYMBOL, MOVE_LEFT, -1);
    add_edge_to_state(q3, q3e0);
    add_edge_to_state(q3, q3e1);
    add_edge_to_state(q3, q3e2);

    struct edge *q4e0 = create_edge(q4, '1', MOVE_LEFT, -1);
    struct edge *q4e1 = create_edge(q5, BLANK_SYMBOL, WRITE, '1');
    add_edge_to_state(q4, q4e1);
    add_edge_to_state(q4, q4e1);

    struct machine *machine = create_machine(q0, 0, &tape[0], 1024);
    while (!is_computation_complete(machine))
    {
        sleep(1);
        print_tape(machine);
        printf("STATE: %d\n", machine->current_state->id);

        int result = step_machine(machine);
        if (result == -1)
        {
            break;
        }
    }
    return 0;
}