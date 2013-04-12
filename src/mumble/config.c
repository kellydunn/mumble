#include <yaml.h>
#include "config.h"

mumble_config_t * new_config(char * filename) {
  yaml_parser_t parser;
  yaml_document_t * document;
  yaml_token_t token;
  yaml_node_t * node;
  mumble_config_t * config;

  yaml_parser_initialize(&parser);

  FILE * file = fopen(filename, "r");
  yaml_parser_set_input_file(&parser, file);
  

  if (!yaml_parser_load(&parser, document)) {
    return NULL;
  }
  
  bool record_next;
  char * record_attr;

  for (node = document->nodes.start; node < document->nodes.top; node ++) {
    switch (node->type) {
    case YAML_SCALAR_NODE:
      if(!record_next && node->data.scalar.value == "monome_path" || node->data.scalar.value == "midi_device") {
        record_next = true;
        record_attr = node->data.scalar.value;

        // TODO Make less sucky
      } else if (record_next) {
        if(record_attr == "monome_path") {
          config->monome_path = node->data.scalar.value;
        } else if (record_attr == "midi_device") {
          config->midi_device = node->data.scalar.value;          
        }

        record_next = false;
      }
      break;
    default:
      break;
    }
  }
      // TODO loop over yaml document
      // if key and == monome_path
      //   record
      // if key and == midi_device
      //   record


  //yaml_document_get_root_node(&document);
  yaml_document_delete(document);
  

  printf("%s | %s \n", config->monome_path, config->midi_device);
  return config;
}
