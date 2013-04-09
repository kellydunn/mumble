#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init(mumble_session_t * session) {
  return NULL;
}

void start_recording(mumble_session_t * session) {
  session->recording = true;
}

void stop_recording(mumble_session_t * session) {
  session->recording = false;
}
