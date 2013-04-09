#include "list.h"

mumble_list_t * mumble_list_new() {
  mumble_list_t * list = calloc(1, sizeof(mumble_list_t));
  list_init(list);
  return list;
}

void list_init(mumble_list_t * list) {
  list->head = NULL;
  list->tail = NULL;

  list->head->prev = (mumble_list_node_t*) &list->tail;
  list->tail->next = (mumble_list_node_t*) &list->head;
}

// Places the passed in val at the begining of the list
void list_prepend(mumble_list_t * list, void * val) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));

  tmp->data = val;

  tmp->next = (mumble_list_node_t*) &list->head;

  list->head = (mumble_list_node_t*) tmp;
  tmp->prev = (mumble_list_node_t*) &list->tail;
}

// Removes and returns the first element in the list
mumble_list_node_t * list_shift(mumble_list_t * list) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));
  
  tmp = (mumble_list_node_t*) &list->head;
  
  list->head = (mumble_list_node_t*) &list->head->next;
  list->head->prev = (mumble_list_node_t*) &list->tail;
  
  tmp->next = NULL;
  tmp->prev = NULL;
  return tmp;
}

// Places the passed in val at the end of the list
void list_append(mumble_list_t * list, void * val) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));

  tmp->data = val;
  
  list->tail->next = tmp;

  tmp->next = (mumble_list_node_t*) &list->head;
  tmp->prev = (mumble_list_node_t*) &list->tail;

  list->tail = tmp;
}

// Removes and returns the last element in the list
mumble_list_node_t * list_pop(mumble_list_t * list) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));
  
  tmp = (mumble_list_node_t*) &list->tail;
  
  list->tail = (mumble_list_node_t*) &list->tail->prev;
  list->tail->next = (mumble_list_node_t*) &list->head;

  tmp->next = NULL;
  tmp->prev = NULL;
  return tmp;
}
