#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "machine.h"
#include "csv_parser.h"

int main()
{
    // This simple turing machine will shift a string right by one and pad with 0.

    struct machine *machine = load_machine_from_file(fopen("machine.csv", "r"));
    if (machine == NULL)
    {
        return 1;
    }

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