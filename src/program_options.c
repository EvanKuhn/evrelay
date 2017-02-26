#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "program_options.h"
#include "version.h"
#include "uv-version.h"

//------------------------------------------------------------------------------
// Command line options and usage string
//------------------------------------------------------------------------------

static const char* USAGE_STR =
  "evrelay: fast metrics relay\n"
  "\n"
  "Usage: evrelay --config-file <file>\n"
  "\n"
  "Options:\n"
  "    -f, --config-file FILE   Start with the given config file\n"
  "    -v, --version            Print the version and exit\n"
  "    -h, --help               Print this usage info\n";

static const char* short_options = "f:vh";

static struct option long_options[] =
  {
    {"config-file", required_argument, 0, 'f'},
    {"version",     no_argument,       0, 'v'},
    {"help",        no_argument,       0, 'h'},
    {0, 0, 0, 0}
  };

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

void program_options_init(program_options_t* options)
{
  memset(options, 0, sizeof(program_options_t));
}

void print_usage()
{
  printf("%s", USAGE_STR);
}

void print_version()
{
  printf("evrelay %s using:\n- libuv %d.%d.%d\n",
    EVRELAY_VERSION,
    UV_VERSION_MAJOR, UV_VERSION_MINOR, UV_VERSION_PATCH);
}

// Parse the command-line options and populate the options struct.
// - May print usage info and exit the program if needed.
void program_options_parse(int argc, char** argv, program_options_t* options)
{
  program_options_init(options);

  // Print usage info and exit if given no args
  if (argc == 1) {
    print_usage();
    exit(0);
  }

  // Disable getopt_long print error messages
  opterr = 0;

  // Parse command-line args
  int opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
    switch (opt) {
      case 'f':
        options->config_file = optarg;
        break;
      case 'v':
        options->version = true;
        break;
      case 'h':
        options->help = true;
        break;
      default:
        fprintf(stderr, "evrelay: unrecognized option '%s'\n", argv[optind-1]);
        exit(1);
    }
  }

  // Handle simple tasks
  if (options->help) {
    print_usage();
    exit(0);
  }
  else if (options->version) {
    print_version();
    exit(0);
  }

  // Check for require inputs
  if (!options->config_file) {
    fprintf(stderr, "evrelay: --config-file required\n");
    exit(1);
  }
}
