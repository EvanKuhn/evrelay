#include <stdio.h>
#include "program_options.h"
#include "relay_config.h"

int main(int argc, char** argv)
{
  // Parse command-line args
  program_options_t options;
  program_options_parse(argc, argv, &options);

  // Parse the config file
  relay_config_t conf;
  relay_config_parse(options.config_file, &conf);

  //...

  return 0;
}
