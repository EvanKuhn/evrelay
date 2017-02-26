#include "relay_config.h"
#include <string.h>

void relay_config_init(relay_config_t* conf)
{
  memset(conf, 0, sizeof(relay_config_t));
}

void relay_config_parse(const char* config_file, relay_config_t* conf)
{
  // Clear the config
  relay_config_init(conf);

  // Parse the file
  // TODO
}
