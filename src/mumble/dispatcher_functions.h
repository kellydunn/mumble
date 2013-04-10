#ifndef _MUMBLE_DISPATCHER_FUNCTIONS_H
#define _MUMBLE_DISPATCHER_FUNCTIONS_H

#include "../types.h"
#include "../mumble.h"

void play_midi(const monome_event_t *e, void *user_data);
void record_midi(const monome_event_t *e, void *user_data);

#endif
