# Turing Machine Simulator

A Turing machine simulator written in C. This program simulates any arbitrary signle tape turing machine containing any number of input and tape alphabet symbols.

## Features
- Graph based state/transition model.
- Dynamic growing tape. The tape starts small and grows on damand as the head movie past its current bounds.
- Turing machines can be created in the specified CSV format using the `load_machine_from_file()` function or created manually in code.

```
dynamic_tape.h / dynamic_tape.c                 tape storage and growth
state.h / state.c                               states and edges
machine.h / machine.c                           keeps track of current state, head, tape
simulation.h / simulation.c                     example program which runs the sim
csv_parser.c / csv_parser.h                     can load a csv formatted machine from a file
  ```

## Notation
As with all turing machine, notation differs.
- When moving from state A to B, the machine can either write a symbol to the tape XOR move the tape head. It cannot do both at the same time.
- The tape is infinite to the right however has a starting point. It is not infinite in both directions like some notation.
- For a computation to be considered complete, the machine must be in an accepting state AND the head must be parked at the left most position of the tape.(You could easily edit this if you wish in the code)
## How to use

Clone the project
```bash
  git clone https://github.com/Matthew-j-Cook/turing-machine-simulator
```

Go to the project directory
```bash
  cd turing-machine-simulator
```

Compile with make
```bash
  make
```

Run the program
```bash
  ./simulation
```
This will load and run an example machine written inside machine.csv. The output will display the internal state of the turing machine as it runs through it's computation.
## Usage/Examples
### CSV format
Creating your own turing machine
```csv
## Double hashtag indicates comments
## Transition Structure:
## [current_state],[read_symbol],[to_state],[R] OR [L] OR [write_symbol]
## R means move the tape right, L for left, any other symbol means write that symbol.
## "_" represents the blank symbol
## Note: first line represents start state and last represents accepting state.

start,0,q2,1
q2,1,q2,R
```
- e.g. start,0,q2,1 would mean when in state start and reading a 0, move to state q2 and write a 1.
- e.g. q2,1,q2,R would mean when in state q2 and reading a 1, move to state q2 and move the tape right.


### Using the CSV parser
Creating and running a machine using the csv parser.
```c
struct tape *tape = create_tape();
populate_tape_with_string(tape, "0110");

struct machine *machine = load_machine_from_file(fopen(argv[1], "r"), tape);
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
## CSV format