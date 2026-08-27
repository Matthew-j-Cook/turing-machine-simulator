#include "machine.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 512
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
        if (strncmp((states[i])->name, name, sizeof(char) * MAX_NAME_LENGTH) == 0)
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
 * @brief Will load states and edges from a csv file and return a machine struct. Requires a tape for the machine to use.
 * Will close the file when done.
 *
 * @param file
 * @return struct machine* OR NULL if something goes wrong.
 */
struct machine *load_machine_from_file(FILE *file, struct tape *tape)
{
    if (file == NULL)
    {
        perror("Failed to open file");
    }
    int error_code = 0; // This should remain 0 by the time we create the machine or we know something was wrong with the CSV format

    struct state *states[MAX_STATES + 1] = {NULL}; // Null terminated array of pointer to states
    char line_buffer[MAX_NAME_LENGTH];             // Read the next field from  read_field into here
    struct state *start_state = NULL;              // Used to create machine later
    int seperator;
    char accepting_state_name[MAX_NAME_LENGTH]; // Stores the accepting states name when found in the csv
    accepting_state_name[0] = '\0';

    // Loop through file
    while (1)
    {
        // Read the 4 fields from this line
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_NAME_LENGTH);
        if (seperator == EOF)
            break;

        // We found the accepting state line!
        if (strncmp(line_buffer, "accepting-state:", strlen("accepting-state:")) == 0)
        {
            // Copy text after "accepting-state:"
            strncpy(accepting_state_name, &line_buffer[strlen("accepting-state:")], MAX_NAME_LENGTH - 1);
            printf("%s\n", accepting_state_name);
            continue;
        }
        // We reached a blank or comment line
        if ((line_buffer[0] == '#' && line_buffer[1] == '#') || seperator == '\n')
        {
            // Continue until next line
            while (seperator != '\n')
            {
                seperator = read_field(file, line_buffer, sizeof(char) * MAX_NAME_LENGTH);
            }
            continue;
        }
        char from_state_name[32];
        strncpy(from_state_name, line_buffer, sizeof(from_state_name));

        char read_symbol;
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_NAME_LENGTH);
        read_symbol = line_buffer[0];

        char to_state_name[32];
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_NAME_LENGTH);
        strncpy(to_state_name, line_buffer, sizeof(to_state_name));

        char tape_action; // either a symbol to write or a direction to move (R/L)
        seperator = read_field(file, line_buffer, sizeof(char) * MAX_NAME_LENGTH);
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

    // Find accepting state by name and actually make it accepting
    int i = 0;
    while (1)
    {
        struct state *current_state;
        current_state = states[i];
        if (current_state == NULL || i > MAX_STATES || accepting_state_name[0] == '\0')
        {
            // no accepting state found
            error_code = 4;
            break;
        }
        if (strncmp(current_state->name, accepting_state_name, MAX_NAME_LENGTH) == 0)
        {
            current_state->is_accepting = 1;
            break;
        }
        i++;
    }
    // If theres no start state, then theres no states at all. We need atleast 1 state for a turing machine
    if (start_state == NULL)
    {
        error_code = 5;
    }
    if (error_code != 0)
    {
        printf("ERROR: Failed to load machine from file. Code: %d\n", error_code);
        return NULL;
    }

    struct machine *machine = create_machine(start_state, 0, tape);
    return machine;
}
