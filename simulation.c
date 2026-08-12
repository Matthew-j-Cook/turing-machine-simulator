#include <stdio.h>
#include "machine.h"
#include "dynamic_tape.h"
#include "csv_parser.h"
#include <string.h>
#include <unistd.h>

static void print_tape(struct tape *tape, int head_position);
static void print_tape(struct tape *tape, int head_position)
{
    const char *prefix = "TAPE: ";
    printf("%s", prefix);
    for (int i = 0; i < tape->tape_size; i++)
    {
        putchar(tape->cells[i]);
    }
    putchar('\n');

    // marker line, aligned under the head position
    for (int i = 0; i < (int)strlen(prefix) + head_position; i++)
    {
        putchar(' ');
    }
    printf("^\n");
}

int main()
{
    // This simple turing machine will add one to a unary number. Note 0 = 1, 1 = 11, 2 = 111, etc.
    struct tape *tape = create_tape();
    char tape_string[7] = "111111";
    populate_tape_with_string(tape, tape_string);

    struct state *q0 = create_state(0, 0);
    struct state *q1 = create_state(1, 0);
    struct state *q2 = create_state(2, 0);
    struct state *q3 = create_state(3, 0);
    struct state *q4 = create_state(4, 1);

    struct edge *q0e0 = create_edge(q1, '1', WRITE, 'a');
    add_edge_to_state(q0, q0e0);

    struct edge *q1e0 = create_edge(q2, 'a', MOVE_RIGHT, -1);
    add_edge_to_state(q1, q1e0);

    struct edge *q2e0 = create_edge(q2, '1', MOVE_RIGHT, -1);
    struct edge *q2e1 = create_edge(q3, BLANK_SYMBOL, WRITE, '1');
    add_edge_to_state(q2, q2e0);
    add_edge_to_state(q2, q2e1);

    struct edge *q3e0 = create_edge(q3, '1', MOVE_LEFT, -1);
    struct edge *q3e1 = create_edge(q4, BLANK_SYMBOL, WRITE, '1');
    add_edge_to_state(q3, q3e0);
    add_edge_to_state(q3, q3e1);

    struct edge *q4e0 = create_edge(q4, 'a', WRITE, '1');
    add_edge_to_state(q3, q4e0);

    // struct machine *machine = load_machine_from_file(fopen("machine.csv", "r"));
    struct machine *machine = create_machine(q0, 0, tape);

    // Advance the machine until it reaches an accepting state or the program is aborted.
    while (!is_computation_complete(machine))
    {
        usleep(100000);
        printf("STATE: %d\n", machine->current_state->id);
        print_tape(machine->tape, machine->head_position);

        int result = step_machine(machine);

        if (result == -1)
        {
            printf("Computation aborted.\n");
            break;
        }
    }

    if (is_computation_complete(machine))
    {
        printf("Computation completed successfully\n");
    }

    printf("STATE: %d\n", machine->current_state->id);
    print_tape(machine->tape, machine->head_position);
    printf("head pos: %d, tape size: %d", machine->head_position, machine->tape->tape_size);

    return 0;
}