#include "list.h"
#include "session.h"

// @return {mumble_session_t*} Returns a new mumble session with expected associated data.
mumble_session_t * new_session(mumble_t * mumble) {
  mumble_session_t * session = calloc(1, sizeof(mumble_session_t));
  session->recording = false;
  session->loops = (mumble_list_t *) new_list();
  session->mumble = mumble;

  // TODO Make this configurable
  //      Default 8 measures of 120 BPM (in milliseconds, could go to nanoseconds for precision)
  session->max_time = (((120/60) * 2) * 1000000);
  return session;
}

void start_recording(mumble_session_t * session) {
  if(session->recording != true) {
    session->recording = true;
    // TODO fire off a thread that will stop recording at the MAX_TIME of this session
    //      Defined as BPM/60 * measures * 1000 (for milliseconds)
    //mumble_loop_t * current_loop = calloc(1, sizeof(mumble_loop_t));
    //current_loop->events = calloc(1, sizeof(mumble_list_t));
    mumble_loop_t * current_loop = new_loop(session);

    session->current_loop = current_loop;
    add_loop(session, current_loop);
    
    pthread_t * session_recording_loop_thread = calloc(1, sizeof(pthread_t));
    pthread_create(session_recording_loop_thread, NULL, session_recording_loop, session);
    pthread_detach(*session_recording_loop_thread);
  }
}

void stop_recording(mumble_session_t * session) {
  session->recording = false;
}

void add_loop(mumble_session_t * session, mumble_loop_t * loop) {
  list_append(session->loops, loop);  
}

void * session_recording_loop(void * data) {
  int now = 0;
  mumble_session_t * session = (mumble_session_t *) data;

  while(now < session->max_time) {
    usleep(100);
    now += 100;
    
    // bail if user kills recording sesion early
    if(session->recording != true) {
      return NULL;
    }
  }

  mumble_midi_event_t * last_event = (mumble_midi_event_t *) session->current_loop->events->tail->data;
  
  if(last_event->delay == 0) {
    last_event->delay = session->max_time - session->current_loop->duration;
  }

  stop_recording(session);
  session->current_loop->looping = true;
  pthread_t * loop_playback_thread = calloc(1, sizeof(pthread_t));

  session->current_loop->thread = loop_playback_thread;
  pthread_create(loop_playback_thread, NULL, loop_playback, session->current_loop);
  pthread_detach(*loop_playback_thread);
  
  return NULL;
}
