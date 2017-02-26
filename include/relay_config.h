#pragma once

// Configuration data for an evrelay
typedef struct relay_config_s {
  //TODO
} relay_config_t;

// Initialize a config struct to empty values
void relay_config_init(relay_config_t* conf);

// Parse the config file and populate the config struct
void relay_config_parse(const char* config_file, relay_config_t* conf);
