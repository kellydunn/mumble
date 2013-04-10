#include "list.h"

mumble_list_t * new_list() {
  mumble_list_t * list = calloc(1, sizeof(mumble_list_t));
  list_init(list);
  return list;
}

void list_init(mumble_list_t * list) {
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

// Places the passed in val at the begining of the list
void list_prepend(mumble_list_t * list, void * val) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));

  tmp->data = val;

  tmp->next = (mumble_list_node_t*) list->head;

  list->head = (mumble_list_node_t*) tmp;
  tmp->prev = (mumble_list_node_t*) list->tail;

  list->size++;
}

// Removes and returns the first element in the list
mumble_list_node_t * list_shift(mumble_list_t * list) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));
  
  tmp = (mumble_list_node_t*) list->head;
  
  list->head = (mumble_list_node_t*) list->head->next;
  list->head->prev = (mumble_list_node_t*) list->tail;
  
  tmp->next = NULL;
  tmp->prev = NULL;

  list->size--;
  return tmp;
}

// Places the passed in val at the end of the list
void list_append(mumble_list_t * list, void * val) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));

  tmp->data = val;

  if(is_empty(list)) {
    list_init_with_one(list, tmp);
   } else {
    // Actual appending logic
    list->tail->next = tmp;
    tmp->prev = (mumble_list_node_t*) list->tail;
    tmp->next = (mumble_list_node_t*) list->head;
    list->tail = tmp;
  }

  list->size++;
}

// Removes and returns the last element in the list
mumble_list_node_t * list_pop(mumble_list_t * list) {
  mumble_list_node_t * tmp = calloc(1, sizeof(mumble_list_node_t));

  if(is_empty(list)) {
    return NULL;
  } else {    
    tmp = (mumble_list_node_t *) list->tail;

    if(list->size == 1) {
      list_nuke(list);
    } else {
      list->tail = (mumble_list_node_t*) list->tail->prev;
      list->tail->next = (mumble_list_node_t*) list->head;
    }

    tmp->next = NULL;
    tmp->prev = NULL;

    list->size--;
    return tmp;
  }
}

// Returns whether or not the list has no nodes
bool is_empty(mumble_list_t * list) {
  return (list->head == NULL && list->tail == NULL);
}

// Initializes the list to have a single node
void list_init_with_one(mumble_list_t * list, mumble_list_node_t * node) {
  list->head = node;
  list->tail = node;

  list->head->next = list->tail;
  list->head->prev = list->tail;

  list->tail->next = list->head;
  list->tail->prev = list->head;
}

// Completely nukes list
void list_nuke(mumble_list_t * list) {
  list->tail = NULL;
  list->head = NULL;  
}
