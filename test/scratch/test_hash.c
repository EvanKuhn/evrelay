#include "hash.h"
#include <stdio.h>
#include <stdbool.h>

void free_key(void* x) {
  printf("  (freeing key '%s')\n", (char*)x);
}

void free_val(void* x) {
  printf("  (freeing key '%s')\n", (char*)x);
}

//==============================================================================
// Main
//==============================================================================

int main(int argc, char** argv)
{
  char* val;

  // Create a new hash
  printf("Creating new hash\n");
  hash_t* h = hash_new(free_key, free_val);
  printf("  Hash has %zu entries\n", hash_size(h));

  // Add some entries
  printf("\nAdding entries\n");
  hash_set_str(h, "key1", "val1");
  hash_set_str(h, "key2", "val2");
  hash_set_str(h, "key3", "val3");
  hash_set_str(h, "key4", "val4");
  printf("  Hash has %zu entries\n", hash_size(h));

  // Test hash_gets
  printf("\nTesting hash_get()\n");

  val = hash_get_str(h, "key1");
  printf("  'key1' has value '%s'\n", (val ? val : "<null>"));

  val = hash_get_str(h, "key2");
  printf("  'key2' has value '%s'\n", (val ? val : "<null>"));

  val = hash_get_str(h, "key3");
  printf("  'key3' has value '%s'\n", (val ? val : "<null>"));

  val = hash_get_str(h, "key4");
  printf("  'key4' has value '%s'\n", (val ? val : "<null>"));

  val = hash_get_str(h, "xxxx");
  printf("  xxxx has value %s\n", (val ? val : "<null>"));

  // Test the hash_iter object
  printf("\nTesting hash_iter\n");
  for(hash_iter_t iter = hash_iter(h); iter.entry; hash_iter_next(&iter)) {
    printf("  '%s' => '%s'\n", (char*)iter.key, (char*)iter.val);
  }

  // Delete a key
  printf("\nDeleting key3\n");
  bool deleted = hash_del_str(h, "key3");
  printf("  key3 %s deleted\n", (deleted ? "was" : "not"));

  for(hash_iter_t iter = hash_iter(h); iter.entry; hash_iter_next(&iter)) {
    printf("  %s => %s\n", (char*)iter.key, (char*)iter.val);
  }
  printf("  Hash has %zu entries\n", hash_size(h));

  printf("\nClearing hash\n");
  hash_clear(h);
  printf("  Hash has %zu entries\n", hash_size(h));

  hash_free(h);
}
