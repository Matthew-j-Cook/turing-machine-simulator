# Turing Machine Simulator

A Turing machine simulator written in C. This program simulates any arbitrary signle tape turing machine containing any number of input and tape alphabet symbols.


## Usage/Examples
### Using the CSV parser
Creating and running a machine using the csv parser.
```c
int main()
{
    struct machine *machine = load_machine_from_file(fopen("machine.csv", "r"));
    if (machine == NULL)
        return 1;
    
    while (!is_computation_complete(machine))
    {
        
        print_machine_state(machine);

        int result = step_machine(machine);

        if (result == -1)
        {
            printf("Computation aborted.\n");
            break;
        }
    }
    print_machine_state(machine);
    printf("head pos: %d, tape size: %d", machine->head_position, machine->tape->tape_size);

    return 0;
}
```

### Manual creation
Creating turing machines without the csv parser.
```c
struct state *state = create_state("name", 1);

struct edge *edge0 = create_edge(state, '0', WRITE, '1');
struct edge *edge1 = create_edge(state, '1', MOVE_RIGHT, '0');
add_edge_to_state(state, edge0);
add_edge_to_state(state, edge1);

struct tape *tape = create_tape();
char tape_string[] = "101";
populatore_tape_with_string(tape, tape_string);

struct machine *machine = create_machine(state, 0, tape);
//Now just step the machine until computation is complete
```