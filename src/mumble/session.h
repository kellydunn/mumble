// This describes a single instance of a user using mumble.
// It is used to determine the length of recording, playback, and samples

#ifndef _MUMBLE_SESSION_H
#define _MUMBLE_SESSION_H

#include "stdlib.h"
#include "stdio.h"

// ===

#include "../types.h"
#include "sample.h"
#include "midi.h"

mumble_session_t * mumble_session_init(mumble_session_t * session);

void start_recording(mumble_session_t * session);
void stop_recording(mumble_session_t * session);
void record_midi_event(mumble_session_t * session, midi_time_event_t * time_event); 

#endif