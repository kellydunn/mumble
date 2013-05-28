#ifndef _MUMBLE_CONFIG_H
#define _MUMBLE_CONFIG_H

#include "../types.h"

mumble_config_t * new_config(char * filename);
mumble_config_node_t * new_config_node();
mumble_list_t * config_from_file(char * filename);
void config_node_append_data(mumble_config_node_t * node, char * data);
bool is_complete(mumble_config_node_t * node);
char * get_config_data(mumble_list_t * list, char * match);

#endif
