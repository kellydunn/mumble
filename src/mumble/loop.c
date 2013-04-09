#include "loop.h"

// Adds a midi event
void add_midi_event(mumble_loop_t * loop, mumble_midi_event_t * event) {
  list_append(&loop->events, event);
}
