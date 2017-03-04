#include "ezyaml.h"
#include "yaml.h"
#include <stdio.h>

// Create and return a newly initialized node
ezyaml_node_t* _ezyaml_node_create()
{
  ezyaml_node_t* node = malloc(sizeof(ezyaml_node_t));
  memset(node, 0, sizeof(ezyaml_node_t));
  return node;
}

void ezyaml_node_delete(ezyaml_node_t* node); // TODO

void _ezyaml_node_set_seq(ezyaml_node_t* node); // TODO

void _ezyaml_node_set_map(ezyaml_node_t* node); // TODO


// TODO: this API can't handle multiple docs per stream
ezyaml_node_t* _process_yaml_events(yaml_parser_t* parser)
{
  bool done = false;
  yaml_event_t event;           // Current event
  ezyaml_node_t* root = NULL;   // Root node of the doc
  ezyaml_node_t* curr = NULL;   // Current node we're operating on
  char* key = NULL;             // The key, when considering a key-value pair


  do {
    if (!yaml_parser_parse(parser, &event)) {
      fprintf(stderr, "Parser error %d\n", parser->error);
      return NULL;
    }

    switch(event.type) {
      case YAML_NO_EVENT:
        break;
      case YAML_STREAM_START_EVENT:
        break;
      case YAML_STREAM_END_EVENT:
        done = true;
        break;
      case YAML_DOCUMENT_START_EVENT:
        root = _ezyaml_node_create();
        curr = root;
        break;
      case YAML_DOCUMENT_END_EVENT:
        // We're not handling multi-document streams. Maybe later.
        done = true;
        break;
      case YAML_ALIAS_EVENT:
        // Not yet supported
        break;
      case YAML_SEQUENCE_START_EVENT:
        // If the current node type is undefined, set it
        if (ezyaml_node_is_undef(curr)
          _ezyaml_node_set_seq(curr);
        else
          ; //TODO
        break;
      case YAML_SEQUENCE_END_EVENT:
        break;
      case YAML_MAPPING_START_EVENT:
        // If the current node type is undefined, set it
        if (ezyaml_node_is_undef(curr)
          _ezyaml_node_set_map(curr);
        else
          ; //TODO
        break;
      case YAML_MAPPING_END_EVENT:
        break;
      case YAML_SCALAR_EVENT:
        break;
    }

    // Delete the event after processing
    yaml_event_delete(&event);
  }
  while (!done);

  return root;
}

ezyaml_node_t* ezyaml_parse_file(char* filepath)
{
  ezyaml_node_t* retval = NULL;
  FILE* fh = NULL;
  bool parser_initialized = false;
  yaml_parser_t parser;

  // Open file for reading
  fh = fopen(filepath, "r");
  if(fh == NULL) {
    fputs("Failed to open file!\n", stderr);
    goto cleanup;
  }

  // Initialize parser
  if(!yaml_parser_initialize(&parser)) {
    fputs("Failed to initialize parser!\n", stderr);
    goto cleanup;
  }
  parser_initialized = true;

  // Parse the yaml file
  yaml_parser_set_input_file(&parser, fh);
  retval = _process_yaml_events(&parser);

  // Clean up and return
cleanup:
  if (parser_initialized)
    yaml_parser_delete(&parser);
  if (fh)
    fclose(fh);
  return retval;
}

bool ezyaml_node_is_undef(ezyaml_node_t* node) { return node->type == NODE_TYPE_UNDEF; }
bool ezyaml_node_is_str  (ezyaml_node_t* node) { return node->type == NODE_TYPE_STR; }
bool ezyaml_node_is_int  (ezyaml_node_t* node) { return node->type == NODE_TYPE_INT; }
bool ezyaml_node_is_flt  (ezyaml_node_t* node) { return node->type == NODE_TYPE_FLT; }
bool ezyaml_node_is_seq  (ezyaml_node_t* node) { return node->type == NODE_TYPE_SEQ; }
bool ezyaml_node_is_map  (ezyaml_node_t* node) { return node->type == NODE_TYPE_MAP; }

char* ezyaml_type_to_string(ezyaml_node_type_t type)
{
  switch (node->type) {
    case NODE_TYPE_STR: return "string";
    case NODE_TYPE_INT: return "integer";
    case NODE_TYPE_FLT: return "float";
    case NODE_TYPE_MAP: return "map";
    case NODE_TYPE_SEQ: return "sequence";
    default:            return "undef";
  }
}

ezyaml_node_t* ezyaml_seq_at(ezyaml_node_t* node, int index);
ezyaml_node_t* ezyaml_map_find(ezyaml_node_t* node, char* key);


char*   ezyaml_str(ezyaml_node_t* node);
int     ezyaml_int(ezyaml_node_t* node);
double  ezyaml_flt(ezyaml_node_t* node);



