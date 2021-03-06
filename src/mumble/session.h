// This describes a single instance of a user using mumble.
// It is used to determine the length of recording, playback, and samples

#ifndef _MUMBLE_SESSION_H
#define _MUMBLE_SESSION_H

#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "unistd.h"

#include "../types.h"
#include "loop.h"
#include "midi.h"

mumble_session_t * new_session();

void start_recording(mumble_session_t * session);
void stop_recording(mumble_session_t * session);

void add_loop(mumble_session_t * session, mumble_loop_t * loop);
void * session_recording_loop(void * session);

#endif
