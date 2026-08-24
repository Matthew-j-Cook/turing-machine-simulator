
## Usage/Examples
### CSV format
Creating your own turing machine
```csv
## Double hashtag indicates comments
## Transition Structure:
## [current_state],[read_symbol],[to_state],[R] || [L] || [write_symbol]
## R means move the tape right, L for left, any other symbol means write that symbol.
## "_" represents the blank symbol
## Note: first line represents start state and last represents accepting state.
```
- e.g. q1,0,q2,1 would mean when in state q1 and reading a 0, move to q2 and write a 1.
- e.g. q1,1,q2,R would mean when in state q1 and reading a 1, move to q1 and move the tape right.

**Note on notation:** when moving from state A to B, the machine can either write a symbol to the tape XOR move the tape head. It cannot do both at the same time like many others. I'm aware this isnt the most common notation but this is how my lecturer taughgt me.
### Using the CSV parser
Creating and running a machine using the csv parser.
```c
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