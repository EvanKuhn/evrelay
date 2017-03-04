#include "yaml.h"
#include <stdio.h>

#define CONFIG_FILE "evrelay.conf"

//==============================================================================
// Token parser
//==============================================================================

void token_parser()
{
  FILE *fh = fopen(CONFIG_FILE, "r");
  yaml_parser_t parser;
  yaml_token_t  token;   /* new variable */

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  /* BEGIN new code */
  do {
    yaml_parser_scan(&parser, &token);
    switch(token.type)
    {
    /* Stream start/end */
    case YAML_STREAM_START_TOKEN:         puts("STREAM START"); break;
    case YAML_STREAM_END_TOKEN:           puts("STREAM END");   break;
    /* Token types (read before actual token) */
    case YAML_KEY_TOKEN:                  printf("(Key token)   "); break;
    case YAML_VALUE_TOKEN:                printf("(Value token) "); break;
    /* Block delimeters */
    case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
    case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
    case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
    /* Data */
    case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
    case YAML_SCALAR_TOKEN:               printf("scalar %s \n", token.data.scalar.value); break;
    /* Others */
    default:
      printf("Got token of type %d\n", token.type);
    }
    if(token.type != YAML_STREAM_END_TOKEN)
      yaml_token_delete(&token);
  } while(token.type != YAML_STREAM_END_TOKEN);
  yaml_token_delete(&token);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);
}

//==============================================================================
// Event parser
//==============================================================================

void event_parser()
{
  FILE *fh = fopen(CONFIG_FILE, "r");
  yaml_parser_t parser;
  yaml_event_t  event;   /* New variable */

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  /* START new code */
  do {
    if (!yaml_parser_parse(&parser, &event)) {
       printf("Parser error %d\n", parser.error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    {
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
    case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
    case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
    case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); break;
    case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");   break;
    case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b>");  break;
    case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b>");    break;
    /* Data */
    case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
    case YAML_SCALAR_EVENT: printf("Got scalar (value %s)\n", event.data.scalar.value); break;
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);
}


//==============================================================================
// Document parser
//==============================================================================

char* node_type_to_string(yaml_node_type_t type)
{
  switch(type) {
    case YAML_NO_NODE:       return "none";
    case YAML_SCALAR_NODE:   return "scalar";
    case YAML_SEQUENCE_NODE: return "sequence";
    case YAML_MAPPING_NODE:  return "mapping";
    default:                 return "???";
  }
}


char* yaml_mark_to_string(yaml_mark_t mark, char* buf, size_t buflen) {
  snprintf(buf, buflen, "[i=%zu l=%zu c=%zu]", mark.index, mark.line, mark.column);
  return buf;
}

void print_node(yaml_node_t* node)
{
  char buf[128];
  size_t buflen = 128;

  printf("node\n");
  printf("- type: %s\n", node_type_to_string(node->type));
  //printf("- tag: %s\n", node->tag);
  printf("- start_mark: %s\n", yaml_mark_to_string(node->start_mark, buf, buflen));
  printf("- end_mark: %s\n", yaml_mark_to_string(node->end_mark, buf, buflen));

  switch (node->type) {
    case YAML_NO_NODE:
      break;
    case YAML_SCALAR_NODE:
      printf("- value = %s\n", node->data.scalar.value);
      break;
    case YAML_SEQUENCE_NODE:
      {
        printf("- sequence = ");
        yaml_node_item_t* t   = node->data.sequence.items.start;
        yaml_node_item_t* end = node->data.sequence.items.end;
        for (; t != end; ++t) {
          printf("%d, ", *t);
        }
        printf("\n");
      }
      break;
    case YAML_MAPPING_NODE:
      printf("- map: \n");
      break;
  }
}

void doc_test()
{
  yaml_parser_t parser;
  yaml_document_t doc;

  // Open file for reading
  FILE *fh = fopen(CONFIG_FILE, "r");
  if(fh == NULL) {
    fputs("Failed to open file!\n", stderr);
    return;
  }

  // Initialize parser
  if(!yaml_parser_initialize(&parser)) {
    fputs("Failed to initialize parser!\n", stderr);
    return;
  }

  // Parse the file to a yaml document
  yaml_parser_set_input_file(&parser, fh);
  if (!yaml_parser_load(&parser, &doc)) {
    fputs("Failed to parse doc\n", stderr);
    return;
  }

  // Print stuff
  printf("\nPrinting yaml node marks\n");
  for (yaml_node_t* n = doc.nodes.start; n && n != doc.nodes.end; ++n) {
    if (n->type == YAML_NO_NODE)
      continue;
    print_node(n);
  }
  printf("done printing nodes\n\n");
}

//==============================================================================
// Main
//==============================================================================

int main(int argc, char** argv)
{
  //token_parser();
  event_parser();
  //doc_test();
  return 0;
}
