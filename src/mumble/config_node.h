#ifndef _MUMBLE_CONFIG_NODE_H
#define _MUMBLE_CONFIG_NODE_H

#include "../types.h"

mumble_config_node_t * new_config_node();
bool config_node_is_complete(mumble_config_node_t * node);
void config_node_append_data(mumble_config_node_t * node, char * data);

#endif
