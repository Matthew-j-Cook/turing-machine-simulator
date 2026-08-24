#include "machine.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 64
#define MAX_LINE_SIZE 1024
#define MAX_NAME_LEN 32
#define DEBUG 0

/**
 * @brief Load line, upto next seperator, into buf. Returns the seperator we hit. Ignores all whitespace
 *
 * @param file
 * @param buf
 * @param max_len
 * @return int //The seperator symbol
 */
static int read_field(FILE *file, char *buf, int max_len)
{
    int i = 0;
    int c;
    while ((c = fgetc(file)) != EOF && c != ',' && c != '\n')
    {
        if (c == '\r' || c == '\t' || c == ' ')
        {
            continue; // Dont include certain whitespace chars
        }
        if (i < max_len - 1)
        {
            buf[i++] = (char)c;
        }
    }
    buf[i] = '\0';
    return c;
}
/**
 * @brief Given an array of pointers to states, check a state corrisponding to the given name exists, return a pointer to that state.
 * If it does not, create and add it to the list before returning it.
 *
 * @param states
 * @param name
 * @param is_accepting
 * @return struct state*
 */
static struct state *create_or_get_state_from_array(struct state **states, char *name, char is_accepting)
{
    int i = 0;
    while (states[i] != NULL)
    {
        if (strncmp((states[i])->name, name, sizeof(char) * MAX_NAME_LEN) == 0)
        {
            return states[i];
        }
        i++;
    }
    if (i >= MAX_STATES)
    {
        printf("ERROR: Too many states, max is %d\n", MAX_STATES);
        return NULL;
    }
    struct state *new_state = create_state(name, is_accepting);
    states[i] = new_state;
    states[i + 1] = NULL; // Null terminate the list
    return new_state;
}

/**
 * @brief Given a CSV file, create states and edges and return a turing machine or NULL if something goes wrong.
 * Will close the file when done.
 *
 * @param file
 * @return struct machine*
 */
struct machine *load_machine_from_file(FILE *file)
{

    if (file == NULL)
    {
        return NULL;
    }
    int error_code = 0; // This should remain 0 by the time we create the machine or we know something was wrong with the CSV format

    struct state *states[MAX_STATES + 1] = {NULL}; // Null terminated array of pointer to states
    char line_buffer[MAX_LINE_SIZE];               // When we call read_field, store it here
    struct state *start_state = NULL;              // Used to create machine later
    int seperator;
    // Loop through file
    while (1)
    {
        // Read the 4 fields from this line
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_LINE_SIZE);
        if (seperator == EOF)
            break;

        // We reached a blank or comment line
        if (line_buffer[0] == '#' && line_buffer[1] == '#' || seperator == '\n')
        {
            // Continue until next line
            while (seperator != '\n')
            {
                seperator = read_field(file, line_buffer, sizeof(char) * MAX_LINE_SIZE);
            }
            continue;
        }
        char from_state_name[32];
        strncpy(from_state_name, line_buffer, sizeof(from_state_name));

        char read_symbol;
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_LINE_SIZE);
        read_symbol = line_buffer[0];

        char to_state_name[32];
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_LINE_SIZE);
        strncpy(to_state_name, line_buffer, sizeof(to_state_name));

        char tape_action; // either a symbol to write or a direction to move (R/L)
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_LINE_SIZE);
        tape_action = line_buffer[0];

        // Any of the fields were empty, or white space
        if (from_state_name[0] == '\0' || read_symbol == '\0' || to_state_name[0] == '\0' || tape_action == '\0')
        {
            error_code = 1;
            break;
        }
        // If the next line is not a new line by this point, the line is too big.
        if (seperator != '\n' && seperator != EOF)
        {
            error_code = 2;
            break;
        }
        struct state *from_state = create_or_get_state_from_array(states, from_state_name, 0);
        struct state *to_state = create_or_get_state_from_array(states, to_state_name, 0);

        // Occurs when the states array is full and we try to add a new state.
        if (from_state == NULL || to_state == NULL)
        {
            error_code = 3;
            break;
        }

        struct edge *edge = NULL;
        switch (tape_action)
        {
        case MOVE_RIGHT_SYMBOL:
            edge = create_edge(to_state, read_symbol, MOVE_RIGHT, '-');
            break;
        case MOVE_LEFT_SYMBOL:
            edge = create_edge(to_state, read_symbol, MOVE_LEFT, '-');
            break;
        default:
            edge = create_edge(to_state, read_symbol, WRITE, tape_action);
            break;
        }
        add_edge_to_state(from_state, edge);
        if (start_state == NULL)
        {
            start_state = from_state;
        }
        // printf("Loaded transition: %s --%c/%c--> %s\n", from_state->name, read_symbol, tape_action, to_state->name);
    }
    fclose(file);
    // Find the last state and make it accepting
    if (start_state != NULL)
    {
        int i = 0;
        while (states[i] != NULL)
        {
            i++;
        }
        states[i - 1]->is_accepting = 1;
    }
    else
    {
        // If theres no start state, then theres no states at all. We need atleast 1 state for a turing machine
        error_code = 4;
    }
    if (error_code != 0)
    {
        printf("ERROR: Failed to load machine from file. Code: %d\n", error_code);
        return NULL;
    }
    // TODO: Add a way to enter custom tapes easier. for now this works.
    struct tape *tape = create_tape();
    char tape_string[10] = "10001";
    populate_tape_with_string(tape, tape_string);
    struct machine *machine = create_machine(start_state, 0, tape);
    return machine;
}
