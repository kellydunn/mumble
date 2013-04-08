#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init(mumble_session_t * session) {
  return NULL;
}

void start_recording(mumble_session_t * session) {
  // TODO Implement
  session->recording = true;
}

void stop_recording(mumble_session_t * session) {
  // TODO Implement
  session->recording = false;
}

void record_midi_event(mumble_session_t * session, midi_time_event_t * time_event) {
  // TODO Implement
}
