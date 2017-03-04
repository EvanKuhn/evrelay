#pragma once

#include <stdbool.h>
#include "hash.h"

// Node data types
typedef enum ezyaml_node_type {
  NODE_TYPE_UNDEF = 0,  // No type yet specified
  NODE_TYPE_STR,        // String
  NODE_TYPE_INT,        // Integer
  NODE_TYPE_FLT,        // Floating point
  NODE_TYPE_MAP,        // Map (string to node)
  NODE_TYPE_SEQ         // Sequence
} ezyaml_node_type_t;

// Forward declare the node struct so it can refer to itself internallys
struct ezyaml_node_s;
typedef struct ezyaml_node_s ezyaml_node_t;

// The node struct has a type and a value of that type
struct ezyaml_node_s {
  ezyaml_node_t* parent;
  ezyaml_node_type_t type;

  union {
    // Scalar value types
    char*   strval;
    int     intval;
    double  fltval;

    // Map of (string,ezyaml_node_t) pairs
    hash_t* map;

    // Sequence of nodes
    struct {
      ezyaml_node_t* items;
      size_t length;
    } seq;

  } data;
};

// Parse a yaml document and get the root node
ezyaml_node_t* ezyaml_parse_file(char* filepath);

// Free a node and all internal data. Will free all child nodes.
void ezyaml_node_delete(ezyaml_node_t* node);

// Check node types
bool ezyaml_node_is_undef(ezyaml_node_t* node);
bool ezyaml_node_is_str  (ezyaml_node_t* node);
bool ezyaml_node_is_int  (ezyaml_node_t* node);
bool ezyaml_node_is_flt  (ezyaml_node_t* node);
bool ezyaml_node_is_seq  (ezyaml_node_t* node);
bool ezyaml_node_is_map  (ezyaml_node_t* node);

// Get child nodes
ezyaml_node_t* ezyaml_seq_at(ezyaml_node_t* node, int index);
ezyaml_node_t* ezyaml_map_find(ezyaml_node_t* node, char* key);

// Get node values
char*   ezyaml_str(ezyaml_node_t* node);
int     ezyaml_int(ezyaml_node_t* node);
double  ezyaml_flt(ezyaml_node_t* node);

// Utilities
char* ezyaml_type_to_string(ezyaml_node_type_t type);
