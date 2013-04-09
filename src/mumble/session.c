#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init() {
  mumble_session_t * session = calloc(1, sizeof(mumble_session_t));
  mumble_loop_t * current_loop = calloc(1, sizeof(mumble_loop_t));
  add_loop(session, current_loop);
  return session;
}

void start_recording(mumble_session_t * session) {
  session->recording = true;
}

void stop_recording(mumble_session_t * session) {
  session->recording = false;
}

void add_loop(mumble_session_t * session, mumble_loop_t * loop) {
  list_append(session->loops, loop);  
}
