#pragma once

#include <stdbool.h>
#include "uthash.h"

//==============================================================================
// A hashtable implementation based internally on uthash.
//==============================================================================

struct hash_s;
typedef struct hash_s hash_t;

// Function typedef for freeing keys or values
typedef void (*free_func_t)(void*);

// Create and return a new hashtable. Takes optional functions to free the key
// and value.
hash_t* hash_new(free_func_t key_free, free_func_t val_free);

// Free the hash and all internal key-value data
void hash_free(hash_t* this);

// Set the given key-value pair. Will overwrite an existing entry.
void hash_set(hash_t* this, void* key, size_t keysize, void* value);

// Get the value for the given key, or NULL if no such entry exists
void* hash_get(hash_t* this, void* key, size_t keysize);

// Delete the entry with the given key. Returns true if an entry was deleted,
// false if not.
bool hash_del(hash_t* this, void* key, size_t keysize);

// Delete all entries, including their keys and values
void hash_clear(hash_t* this);

// Get the number of entries in the hash table
size_t hash_size(hash_t* this);

// Utilities for string-based keys
void  hash_set_str(hash_t* this, char* key, void* value);
void* hash_get_str(hash_t* this, char* key);
bool  hash_del_str(hash_t* this, char* key);

//==============================================================================
// Hash table iterator struct. Use it to iterate over a hash_t struct. Example:
//
//     hash_t* hash = hash_new(NULL, NULL);
//     ...
//     for(hash_iter_t iter = hash_iter(hash); iter->entry; hash_iter_next(iter)) {
//       printf("key '%s' has value '%s'\n", (char*)iter->key, (char*)iter->val);
//     }
//
// To check if the iterator is done (ie. no more values are available), you can
// check if iter->entry is null.
//==============================================================================

struct hash_iter_s {
  void* entry;  // Opaque pointer to current hash entry. Null if no more entries.
  void* key;    // Current key
  void* val;    // Current value
};
typedef struct hash_iter_s hash_iter_t;

// Return an initialized iterator for a hash. Calls hash_iter_init internally.
hash_iter_t hash_iter(hash_t* hash);

// Initialize a hash_iter object to point to the first key-value pair
void hash_iter_init(hash_iter_t* this, hash_t* hash);

// Iterate to the next key-value pair. Returns true if such a pair exists, or
// false if no more entries exist.
bool hash_iter_next(hash_iter_t* this);

//TODO: void hash_iter_delete: delete current entry
//TODO: void hash_sort(sort_func): sort the hash by key
