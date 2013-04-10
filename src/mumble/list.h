#ifndef _MUMBLE_LIST_H
#define _MUMBLE_LIST_H

#include "../types.h"

mumble_list_t * new_list();

void list_append(mumble_list_t * list, void * val);

bool is_empty(mumble_list_t * list);

void list_init_with_one(mumble_list_t *list, mumble_list_node_t * node);
#endif
