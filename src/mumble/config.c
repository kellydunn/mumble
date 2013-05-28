#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "list.h"
#include "config.h"

// @param {char*} filename.  An absolute path to the configuration file for the mumble utility.
// @return {mumble_config_t*} config.  The configuration struct in which to configure this session.
mumble_config_t * new_config(char * filename) {
  mumble_config_t * config = (mumble_config_t *) calloc(1, sizeof(mumble_config_t));

  mumble_list_t * config_elements = config_from_file(filename);

  config->monome_path = get_config_data(config_elements, "monome_path");
  config->midi_device = get_config_data(config_elements, "midi_device");

  return config;
}

// @param {char*} filename.  The absolute path to the configuration file for the mumble utility.
// @return {mumble_list_t*} config_elements.  The list of configuration options for the associated mumble session.
mumble_list_t * config_from_file(char * filename) {
  yaml_parser_t parser;
  yaml_token_t token;
  yaml_document_t * document;

  mumble_list_t * config_elements = (mumble_list_t *) new_list();

  FILE * file = fopen(filename, "rb");

  yaml_parser_initialize(&parser);
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

  return config_elements;
}

// @return {mumble_config_node_t*} node.  A new configuration node populated with key and value data.
mumble_config_node_t * new_config_node() {
  mumble_config_node_t * node = (mumble_config_node_t *) calloc(1, sizeof(mumble_config_node_t));
  node->key = NULL;
  node->value = NULL;
}

// @param {mumble_config_node_t*} node. The configuration node in which to add data.
// @param {char*} data. The data value in which to append to the node.  
//                      If no key in the node exists, then the passed in data shall be attached as the key.
//                      If a key in the node exists, then the passed in data shall be attached as the value.
void config_node_append_data(mumble_config_node_t * node, char * data) {
  if(node->key == NULL) {
    node->key = data;
  } else {
    node->value = data;
  }
}

// @param {bool}.  Whether or not the current configuration node has both a key and a value.
bool is_complete(mumble_config_node_t * node) {
  return (node->key != NULL && node->value != NULL);
}

// TODO this is kinda nasty; wish I didn't have to iterate over a list of nodes dumbly.
//      lets make this better in the future by introducing a more intelligent data structure.
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
