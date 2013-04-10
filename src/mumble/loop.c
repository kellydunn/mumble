#include "loop.h"

mumble_loop_t * new_loop(mumble_session_t * session) {
  mumble_loop_t * loop = calloc(1, sizeof(mumble_loop_t));
  loop->looping = false;
  loop->inst = 0; // TODO grab from mumble session
  loop->events = calloc(1, sizeof(mumble_list_t));
}

// Adds a midi event
void add_midi_event(mumble_loop_t * loop, mumble_midi_event_t * event) {
  printf("Adding midi event!\n");
  list_append(loop->events, event);
}
