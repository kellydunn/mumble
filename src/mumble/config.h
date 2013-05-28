#ifndef _MUMBLE_CONFIG_H
#define _MUMBLE_CONFIG_H

#include "../types.h"

mumble_config_t * new_config(char * filename);
mumble_list_t * config_from_file(char * filename);
char * get_config_data(mumble_list_t * list, char * match);

#endif
