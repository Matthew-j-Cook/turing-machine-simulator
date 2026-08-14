#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "machine.h"
#include "csv_parser.h"

int main()
{
    // This simple turing machine will shift a string right by one and pad with 0.
    struct tape *tape = create_tape();
    char tape_string[12] = "11010101011";
    populate_tape_with_string(tape, tape_string);

    struct state *q0 = create_state(0, 0);
    struct state *q1 = create_state(1, 0);
    struct state *q2 = create_state(2, 0);
    struct state *q3 = create_state(3, 0);
    struct state *q4 = create_state(4, 0);
    struct state *q5 = create_state(5, 0);
    struct state *q6 = create_state(6, 1);
    struct state *q7 = create_state(7, 0);

    struct edge *q7e0 = create_edge(q1, '0', WRITE, 'O');
    struct edge *q7e1 = create_edge(q2, '1', WRITE, 'I');
    add_edge_to_state(q7, q7e0);
    add_edge_to_state(q7, q7e1);

    struct edge *q0e0 = create_edge(q1, '0', MOVE_RIGHT, 'a');
    struct edge *q0e1 = create_edge(q2, '1', MOVE_RIGHT, 'a');
    add_edge_to_state(q0, q0e0);
    add_edge_to_state(q0, q0e1);

    struct edge *q1e0 = create_edge(q1, 'O', MOVE_RIGHT, 'a');
    struct edge *q1e1 = create_edge(q4, '1', WRITE, '0');
    struct edge *q1e2 = create_edge(q0, '0', WRITE, '0');
    struct edge *q1e3 = create_edge(q5, BLANK_SYMBOL, WRITE, '0');
    add_edge_to_state(q1, q1e0);
    add_edge_to_state(q1, q1e1);
    add_edge_to_state(q1, q1e2);
    add_edge_to_state(q1, q1e3);

    struct edge *q2e0 = create_edge(q2, 'I', MOVE_RIGHT, 'a');
    struct edge *q2e1 = create_edge(q3, '0', WRITE, '1');
    struct edge *q2e2 = create_edge(q0, '1', WRITE, '1');
    struct edge *q2e3 = create_edge(q5, BLANK_SYMBOL, WRITE, '1');
    add_edge_to_state(q2, q2e0);
    add_edge_to_state(q2, q2e1);
    add_edge_to_state(q2, q2e2);
    add_edge_to_state(q2, q2e3);

    struct edge *q3e0 = create_edge(q1, '1', MOVE_RIGHT, 'a');
    add_edge_to_state(q3, q3e0);

    struct edge *q4e0 = create_edge(q2, '0', MOVE_RIGHT, 'a');
    add_edge_to_state(q4, q4e0);

    struct edge *q5e0 = create_edge(q5, '1', MOVE_LEFT, 'O');
    struct edge *q5e1 = create_edge(q5, '0', MOVE_LEFT, 'O');
    struct edge *q5e2 = create_edge(q6, 'I', WRITE, '0');
    struct edge *q5e3 = create_edge(q6, 'O', WRITE, '0');
    add_edge_to_state(q5, q5e0);
    add_edge_to_state(q5, q5e1);
    add_edge_to_state(q5, q5e2);
    add_edge_to_state(q5, q5e3);

    // struct machine *machine = load_machine_from_file(fopen("machine.csv", "r"));
    struct machine *machine = create_machine(q7, 0, tape);

    // Advance the machine until it reaches an accepting state or the program is aborted.
    int step = 0;
    while (!is_computation_complete(machine))
    {
        usleep(80000);
        printf("STEP: %d\n", step);
        print_machine_state(machine);
        step++;

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
    printf("STEP: %d\n", step);
    print_machine_state(machine);
    printf("head pos: %d, tape size: %d", machine->head_position, machine->tape->tape_size);

    return 0;
}