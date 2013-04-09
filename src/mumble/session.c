#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init() {
  mumble_session_t * session = calloc(1, sizeof(mumble_session_t));
  session->recording = false;
  session->loops = calloc(1, sizeof(mumble_list_t));

  // TODO Make this configurable
  //      Default 8 measures of 120 BPM
  session->max_time = (((120/60) * 2) * 1000);
  return session;
}

void start_recording(mumble_session_t * session) {
  printf("start recording!\n");
  if(session->recording != true) {
    session->recording = true;
    // TODO fire off a thread that will stop recording at the MAX_TIME of this session
    //      Defined as BPM/60 * measures * 1000 (for milliseconds)
    mumble_loop_t * current_loop = calloc(1, sizeof(mumble_loop_t));
    current_loop->events = calloc(1, sizeof(mumble_list_t));
    add_loop(session, current_loop);
    
    pthread_t * session_loop_thread = calloc(1, sizeof(pthread_t));
    pthread_create(session_loop_thread, NULL, session_loop, session);
    pthread_detach(session_loop_thread);
  }
}

void stop_recording(mumble_session_t * session) {
  session->recording = false;
  printf("STOPPED RECORDING");
}

void add_loop(mumble_session_t * session, mumble_loop_t * loop) {
  printf("adding loop!\n");
  list_append(session->loops, loop);  
}

void * session_loop(void * data) {
  int now = 0;
  mumble_session_t * session = (mumble_session_t *) data;
  printf("Starting loop! Start: %d. MAX TIME IS %d\n", now, session->max_time);
  while(now < session->max_time) {
    sleep(1);
    now += 1;
    
    // bail if user kills recording sesion early
    if(session->recording != true) {
      return NULL;
    }
  }

  printf("MAX RECORDING TIME.  EXITING LOOP!\n");
  printf("  You recorded %d events\n", session->current_loop->events->size);
  stop_recording(session);
  return NULL;
}
