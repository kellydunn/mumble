#ifndef _MUMBLE_LOOP_H
#define _MUMBLE_LOOP_H

#include "../types.h"

mumble_loop_t * new_loop(mumble_session_t * session);
void add_event(mumble_loop_t * loop, mumble_midi_event_t * event);
void * loop_playback(void * args);
void loop_playback_midi_event(const monome_event_t * e, void * user_data);

#endif
