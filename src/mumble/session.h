// This describes a single instance of a user using mumble.
// It is used to determine the length of recording, playback, and samples

#ifndef _MUMBLE_SESSION_H
#define _MUMBLE_SESSION_H

#include "stdlib.h"
#include "stdio.h"

// ===

#include "../types.h"
#include "loop.h"
#include "midi.h"

mumble_session_t * mumble_session_init();

void start_recording(mumble_session_t * session);
void stop_recording(mumble_session_t * session);

void add_loop(mumble_session_t * session, mumble_loop_t * loop);

#endif
