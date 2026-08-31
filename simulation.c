#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "machine.h"
#include "csv_parser.h"
#define STEP_DELAY 8000

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        printf("Usage: %s <machine-csv-path> <tape-string>\n", argv[0]);
        return 1;
    }

    struct tape *tape = create_tape();
    populate_tape_with_string(tape, argv[2]);

    struct machine *machine = load_machine_from_file(fopen(argv[1], "r"), tape);
    // Failed to make machine. Probably incorrectly formatted CSV.
    if (machine == NULL)
    {
        return 1;
    }

    int step = 0;
    // Advance the machine until it reaches an accepting state or the program is aborted.
    while (!is_computation_complete(machine))
    {
        printf("STEP: %d\n", step);
        step++;

        print_machine_state(machine);

        int result = step_machine(machine);

        if (result == -1)
        {
            printf("Computation aborted.\n");
            break;
        }
        usleep(STEP_DELAY);
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