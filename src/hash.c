#include "hash.h"

// An entry in the hashtable
typedef struct _entry_s {
    void* key;                // Pointer to key
    void* val;                // Pointer to value
    UT_hash_handle hh;        // For uthash
} _entry_t;

// Hash struct
struct hash_s {
  _entry_t* entries;          // The uthash hashtable
  free_func_t key_free_func;  // Function to free keys
  free_func_t val_free_func;  // Function to free values
};

//==============================================================================
// hash_t
//==============================================================================

// Utility function to lookup an entry
static _entry_t* _hash_find_entry(hash_t* this, void* key, size_t keysize)
{
  _entry_t* entry = NULL;
  HASH_FIND(hh, this->entries, &key, keysize, entry);
  return entry;
}

hash_t* hash_new(free_func_t key_free, free_func_t val_free)
{
  hash_t* h = malloc(sizeof(hash_t));
  h->entries = NULL;
  h->key_free_func = key_free;
  h->val_free_func = val_free;
  return h;
}

void hash_free(hash_t* this)
{
  hash_clear(this);
  free(this);
}

void hash_set(hash_t* this, void* key, size_t keysize, void* value)
{
  // Look for an existing entry
  _entry_t* entry = _hash_find_entry(this, key, keysize);

  // If we already have an entry, free the value and add the new value
  if (entry) {
    if (this->val_free_func)
      this->val_free_func(entry->val);
    entry->val = value;
  }
  // Otherwise, add a new entry
  else {
    entry = malloc(sizeof(_entry_t));
    memset(entry, 0, sizeof(_entry_t));
    entry->key = key;
    entry->val = value;
    HASH_ADD(hh, this->entries, key, keysize, entry);
  }
}

void* hash_get(hash_t* this, void* key, size_t keysize)
{
  _entry_t* entry = _hash_find_entry(this, key, keysize);
  return entry ? entry->val : NULL;
}

size_t hash_size(hash_t* this)
{
  return HASH_COUNT(this->entries);
}

bool hash_del(hash_t* this, void* key, size_t keysize)
{
  // Look for an existing entry
  _entry_t* entry = _hash_find_entry(this, key, keysize);

  // If found, remove it and free everything
  if (entry) {
    HASH_DEL(this->entries, entry);
    if (this->key_free_func) this->key_free_func(entry->key);
    if (this->val_free_func) this->val_free_func(entry->val);
    free(entry);
    return true;
  }
  return false;
}

void hash_clear(hash_t* this)
{
  _entry_t *entry, *temp;
  HASH_ITER(hh, this->entries, entry, temp) {
    HASH_DEL(this->entries, entry);
    if (this->key_free_func) this->key_free_func(entry->key);
    if (this->val_free_func) this->val_free_func(entry->val);
    free(entry);
  }
}

// Utilities for string-based keys
void hash_set_str(hash_t* this, char* key, void* value)
{
  hash_set(this, key, strlen(key), value);
}

void* hash_get_str(hash_t* this, char* key)
{
  return hash_get(this, key, strlen(key));
}


bool hash_del_str(hash_t* this, char* key)
{
  return hash_del(this, key, strlen(key));
}

//==============================================================================
// hash_iter_t
//==============================================================================

hash_iter_t hash_iter(hash_t* hash)
{
  hash_iter_t iter;
  hash_iter_init(&iter, hash);
  return iter;
}

void hash_iter_init(hash_iter_t* this, hash_t* hash)
{
  this->entry = hash->entries;
  this->key = ((_entry_t*)this->entry)->key;
  this->val = ((_entry_t*)this->entry)->val;
}

bool hash_iter_next(hash_iter_t* this)
{
  // Check for current entry
  _entry_t* e = this->entry;
  if (!e) return false;

  // Get next entry (could be null)
  e = e->hh.next;
  this->entry = e;

  // Update key and value
  this->key = e ? e->key : NULL;
  this->val = e ? e->val : NULL;
  return true;
}
