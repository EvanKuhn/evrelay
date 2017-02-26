#pragma once

#include <stdbool.h>

typedef struct program_options_s {
  bool help;
  bool version;
  char* config_file;
} program_options_t;

void program_options_init(program_options_t* options);

// Parse the command-line options and populate the options struct.
// - Will initialize the options struct.
// - May print usage info and exit the program if needed.
void program_options_parse(int argc, char** argv, program_options_t* options);

// Print the usage string
void print_usage();

// Print evrelay version and library versions
void print_version();
