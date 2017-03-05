#include "ezyaml.h"
#include "yaml.h"
#include <stdio.h>
#include <string.h>

//==============================================================================
// Utilities
//==============================================================================

// Convert a string to a double
// - If successful, store the numeric value to 'd', and return true.
// - On error, return false.
bool _str_to_double(char* s, double* d)
{
  char* endptr = s + strlen(s);
  *d = strtod(s, &endptr);
  return (endptr != s);
}

// Convert a string to an integer.
// - If successful, store the numeric value to 'i', and return true.
// - On error, return false.
bool _str_to_int(char* s, int* i)
{
  *i = atoi(s);
  return (*i != 0 || strcmp(s, "0") == 0);
}

// hash_t-compatible free function
void _node_free(void* p)
{
  ezyaml_node_t* node = p;
  ezyaml_node_delete(node);
}

//==============================================================================
// Node creation / setter / getter functions
//==============================================================================

// Set a node as a scalar. Takes a string value and converts to integer or
// float where possible.
void _ezyaml_node_set_scalar(ezyaml_node_t* node, char* value)
{
  double d;
  int i;
  if (_str_to_double(value, &d)) {
    node->type = NODE_TYPE_FLT;
    node->data.fltval = d;
  }
  else if (_str_to_int(value, &i)) {
    node->type = NODE_TYPE_INT;
    node->data.intval = i;
  }
  else {
    node->type = NODE_TYPE_STR;
    node->data.strval = value;
  }
}

void _ezyaml_node_set_seq(ezyaml_node_t* node)
{
  node->type = NODE_TYPE_SEQ;
  node->data.seq.items = NULL;
  node->data.seq.length = 0;
}

void _ezyaml_node_set_map(ezyaml_node_t* node)
{
  node->type = NODE_TYPE_MAP;
  node->data.map = hash_new(free, _node_free);
}

// Create and return a newly initialized node
ezyaml_node_t* _ezyaml_node_create()
{
  ezyaml_node_t* node = malloc(sizeof(ezyaml_node_t));
  memset(node, 0, sizeof(ezyaml_node_t));
  return node;
}

ezyaml_node_t* _ezyaml_node_create_scalar(char* value)
{
  ezyaml_node_t* node = _ezyaml_node_create();
  _ezyaml_node_set_scalar(node, value);
  return node;
}

ezyaml_node_t* _ezyaml_node_create_seq()
{
  ezyaml_node_t* node = _ezyaml_node_create();
  _ezyaml_node_set_seq(node);
  return node;
}

ezyaml_node_t* _ezyaml_node_create_map()
{
  ezyaml_node_t* node = _ezyaml_node_create();
  _ezyaml_node_set_map(node);
  return node;
}

void ezyaml_node_delete(ezyaml_node_t* node)
{
  switch (node->type) {
    case NODE_TYPE_STR:
      free(node->data.strval);
      break;
    case NODE_TYPE_SEQ:
      for (size_t i = 0; i < node->data.seq.length; i++) {
        ezyaml_node_delete(node->data.seq.items[i]);
      }
      free(node->data.seq.items);
      break;
    case NODE_TYPE_MAP:
      hash_free(node->data.map);
      break;
    default:
      break;
  }
  memset(node, 0, sizeof(ezyaml_node_t));
}

// Functions for checking node type
inline bool ezyaml_node_is_undef(ezyaml_node_t* node) { return node->type == NODE_TYPE_UNDEF; }
inline bool ezyaml_node_is_int  (ezyaml_node_t* node) { return node->type == NODE_TYPE_INT; }
inline bool ezyaml_node_is_flt  (ezyaml_node_t* node) { return node->type == NODE_TYPE_FLT; }
inline bool ezyaml_node_is_str  (ezyaml_node_t* node) { return node->type == NODE_TYPE_STR; }
inline bool ezyaml_node_is_seq  (ezyaml_node_t* node) { return node->type == NODE_TYPE_SEQ; }
inline bool ezyaml_node_is_map  (ezyaml_node_t* node) { return node->type == NODE_TYPE_MAP; }

char* ezyaml_type_to_string(ezyaml_node_type_t type)
{
  switch (type) {
    case NODE_TYPE_INT: return "integer";
    case NODE_TYPE_FLT: return "float";
    case NODE_TYPE_STR: return "string";
    case NODE_TYPE_SEQ: return "sequence";
    case NODE_TYPE_MAP: return "map";
    default:            return "undef";
  }
}

// Append a new node to the sequence.
// - If the given node is not a sequence type, do nothing.
void _ezyaml_node_seq_append(ezyaml_node_t* node, ezyaml_node_t* appendme)
{
  if (!ezyaml_node_is_seq(node))
    return;

  // Create the array if it doesn't already exist
  if (node->data.seq.items == NULL) {
    node->data.seq.items = calloc(2, sizeof(ezyaml_node_t*));
  }
  // Resize the array if it needs resizing
  else {
    size_t capacity = sizeof(node->data.seq.items) / sizeof(ezyaml_node_t*);
    if (node->data.seq.length == capacity) {
      size_t newsize = 2 * capacity * sizeof(ezyaml_node_t*);
      node->data.seq.items = realloc(node->data.seq.items, newsize);
    }
  }

  // Append the item
  node->data.seq.items[node->data.seq.length++] = appendme;
}

ezyaml_node_t* ezyaml_seq_at(ezyaml_node_t* node, int index)
{
  return node->data.seq.items[index];
}

ezyaml_node_t* ezyaml_map_find(ezyaml_node_t* node, char* key)
{
  return hash_get_str(node->data.map, key);
}

int ezyaml_int(ezyaml_node_t* node)
{
  return (node->type == NODE_TYPE_INT ? node->data.intval : 0);
}

double ezyaml_flt(ezyaml_node_t* node)
{
  return (node->type == NODE_TYPE_FLT ? node->data.fltval : 0.0);
}

char* ezyaml_str(ezyaml_node_t* node)
{
  return (node->type == NODE_TYPE_STR ? node->data.strval : NULL);
}

//==============================================================================
// YAML parsing functions
//==============================================================================

ezyaml_node_t* _process_yaml_events(yaml_parser_t* parser)
{
  bool done = false;
  yaml_event_t event;           // Current event
  ezyaml_node_t* root = NULL;   // Root node of the doc
  ezyaml_node_t* curr = NULL;   // Current node we're operating on
  char* key = NULL;             // The key, when considering a key-value pair

  do {
    // Get the next event
    if (!yaml_parser_parse(parser, &event)) {
      fprintf(stderr, "Parser error %d\n", parser->error);
      goto error;
    }

    switch(event.type) {
      case YAML_NO_EVENT:
        break;

      //------------------------------------------------------------------------
      // Stream
      //------------------------------------------------------------------------
      case YAML_STREAM_START_EVENT:
        break;

      case YAML_STREAM_END_EVENT:
        done = true;
        break;

      //------------------------------------------------------------------------
      // Document
      //------------------------------------------------------------------------
      case YAML_DOCUMENT_START_EVENT:
        root = _ezyaml_node_create();
        curr = root;
        break;

      case YAML_DOCUMENT_END_EVENT:
        // TODO: add support for multiple documents per stream
        done = true;
        break;

      //------------------------------------------------------------------------
      // Alias
      //------------------------------------------------------------------------
      case YAML_ALIAS_EVENT:
        // TODO: add support for aliases
        break;

      //------------------------------------------------------------------------
      // Scalar
      //------------------------------------------------------------------------
      case YAML_SCALAR_EVENT:
        if (curr) { //TODO: we should error out if 'curr' is not set
          char* scalar_value = (char*)event.data.scalar.value;

          // If the current node type is undefined, we must have just started
          // the document. In this case, the doc must only consist of a single
          // scalar value.
          if (ezyaml_node_is_undef(curr)) {
            _ezyaml_node_set_scalar(curr, scalar_value);
            curr = NULL; // Indicates that there can be no more values in the doc
          }
          // Current node is a sequence. Add the scalar value to it.
          else if (ezyaml_node_is_seq(curr)) {
            ezyaml_node_t* newnode = _ezyaml_node_create_scalar(scalar_value);
            _ezyaml_node_seq_append(curr, newnode);
          }
          // Current node is a map. If we have the key, insert the key-value
          // pair. Otherwise, just save the key.
          else if (ezyaml_node_is_map(curr)) {
            if (key) {
              ezyaml_node_t* newnode = _ezyaml_node_create_scalar(scalar_value);
              hash_set_str(curr->data.map, key, newnode);
              key = NULL;
            }
            else {
              key = strdup(scalar_value);
            }
          }
          else {
            // TODO: parse error! what type is the current node??
          }
        }
        break;

      //------------------------------------------------------------------------
      // Sequence
      //------------------------------------------------------------------------
      case YAML_SEQUENCE_START_EVENT:
        // TODO: error out if 'curr' is not set

        // If the current node type is undefined, we must have just started
        // the document. Set the current (root) node to a sequence.
        if (ezyaml_node_is_undef(curr)) {
          _ezyaml_node_set_seq(curr);
        }
        // If current the node is a sequence, add a new node to the sequence and
        // make that the new current node.
        else if (ezyaml_node_is_seq(curr)) {
          // TODO
        }
        // If current the node is a map, we expect to have a key. If so, add the
        // key-sequence pair to the map and set the new sequence node as the
        // current node.
        else if (ezyaml_node_is_map(curr)) {

        }
        else {
          // TODO: parse error! what type is the current node??
        }
        break;

      case YAML_SEQUENCE_END_EVENT:
        break;

      //------------------------------------------------------------------------
      // Mapping
      //------------------------------------------------------------------------
      case YAML_MAPPING_START_EVENT:
        // If the current node type is undefined, set it
        if (ezyaml_node_is_undef(curr))
          _ezyaml_node_set_map(curr);
        else
          ; //TODO
        break;

      case YAML_MAPPING_END_EVENT:
        break;
    }

    // Delete the event after processing
    yaml_event_delete(&event);
  }
  while (!done);

  // Success! Return the root node.
  return root;

  // On error, delete any existing nodes and return null
error:
  if (root)
    ezyaml_node_delete(root);
  return NULL;
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
