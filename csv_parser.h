#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <stdio.h>

// Will load states and edges from a csv file and return a machine struct. Requires a tape for the machine to use.
// Will close the file when done.
struct machine *load_machine_from_file(FILE *file, struct tape *tape);

#endif // CSV_PARSER_H