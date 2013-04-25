#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "list.h"
#include "config.h"

mumble_config_t * new_config(char * filename) {
  yaml_parser_t parser;
  yaml_token_t token;
  yaml_document_t * document;
  mumble_config_t * config = (mumble_config_t *) calloc(1, sizeof(mumble_config_t));

  mumble_list_t * config_elements = (mumble_list_t *) new_list();
  bool capture = false;
  
  yaml_parser_initialize(&parser);
  FILE * file = fopen(filename, "rb");
  yaml_parser_set_input_file(&parser, file);
  
  mumble_config_node_t * node = new_config_node();

  do {
    if(is_complete(node)) {
      list_append(config_elements, node);
      node = new_config_node();
    }

    yaml_parser_scan(&parser, &token);
    switch(token.type) {
    case YAML_SCALAR_TOKEN:
      // if(strdup((const char *) token.data.scalar.value) != NULL) {
      if(token.data.scalar.value != NULL) {
        config_node_append_data(node, strdup((const char *)token.data.scalar.value));
      }
      break;
    }

    if(token.type != YAML_STREAM_END_TOKEN) {
      yaml_token_delete(&token);
    }
  } while(token.type != YAML_STREAM_END_TOKEN);

  yaml_token_delete(&token);

  config->monome_path = get_config_data(config_elements, "monome_path");
  config->midi_device = get_config_data(config_elements, "midi_device");

  return config;
}

mumble_config_node_t * new_config_node() {
  mumble_config_node_t * node = (mumble_config_node_t *) calloc(1, sizeof(mumble_config_node_t));
  node->key = NULL;
  node->value = NULL;
}

void config_node_append_data(mumble_config_node_t * node, char * data) {
  if(node->key == NULL) {
    node->key = data;
  } else {
    node->value = data;
  }
}

bool is_complete(mumble_config_node_t * node) {
  return (node->key != NULL && node->value != NULL);
}

// TODO this is kinda nasty; wish I didn't have to iterate over a list of nodes dumbly.
//      lets make this better in the future.
char * get_config_data(mumble_list_t * list, char * match) {
  mumble_list_node_t * current;
  current = list->head;
  int i;
  
  for(i = 0; i < list->size; i++) {
    mumble_config_node_t * node = current->data;
    if(strcmp(node->key, match) == 0) {
      return node->value;
    }
    current = current->next;
  }

  return NULL;
}
