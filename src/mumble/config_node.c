// This class reprents a temporary data structure used to keep track of configuration options for mumble
// Each node is essentially a key-value pair. 
// The key memeber is used as an index in searching for {mumble_config_node_t}.
// The value member is the interesting data associated with the key memeber.

#include "config_node.h"

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
bool config_node_is_complete(mumble_config_node_t * node) {
  return (node->key != NULL && node->value != NULL);
}
