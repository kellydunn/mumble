#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init() {
  mumble_session_t * session = calloc(1, sizeof(mumble_session_t));
  return session;
}

void start_recording(mumble_session_t * session) {
  if(!session->recording) {
    session->recording = true;
    // TODO fire off a thread that will stop recording at the MAX_TIME of this session
    //      Defined as BPM/60 * measures (in seconds)
    mumble_loop_t * current_loop = calloc(1, sizeof(mumble_loop_t));
    add_loop(session, current_loop);
  }
}

void stop_recording(mumble_session_t * session) {
  session->recording = false;
}

void add_loop(mumble_session_t * session, mumble_loop_t * loop) {
  list_append(session->loops, loop);  
}
