#include "session.h"

// TODO Implement
mumble_session_t * mumble_session_init(mumble_t * mumble) {
  mumble_session_t * session = calloc(1, sizeof(mumble_session_t));
  session->recording = false;
  session->loops = new_list();
  session->mumble = mumble;

  // TODO Make this configurable
  //      Default 8 measures of 120 BPM (in milliseconds, could go to nanoseconds for precision)
  session->max_time = (((120/60) * 2) * 10000);
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
    pthread_detach(session_recording_loop_thread);
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
  printf("Starting loop! Start: %d. MAX TIME IS %d\n", now, session->max_time);
  while(now < session->max_time) {
    usleep(1);
    now += 1;
    
    // bail if user kills recording sesion early
    if(session->recording != true) {
      return NULL;
    }
  }

  printf("MAX RECORDING TIME.  EXITING LOOP!\n");
  printf("  You recorded %d events\n", session->current_loop->events->size);

  // DEBUG EVENTS PLZ
  mumble_list_node_t * debug = calloc(1, sizeof(mumble_list_node_t));
  debug = session->current_loop->events->head;
  mumble_midi_event_t * ddata = (mumble_midi_event_t *) debug->data;
  printf("[%d, %d, %d] =>", ddata->monome_event->event_type, ddata->monome_event->grid.x, ddata->monome_event->grid.y);

  while((debug = debug->next) != session->current_loop->events->head) {
    ddata = (mumble_midi_event_t *) debug->data;
    printf("[%d, %d, %d] =>", ddata->monome_event->event_type, ddata->monome_event->grid.x, ddata->monome_event->grid.y);
  }

  printf("\n");

  stop_recording(session);

  // TODO Setup a semaphore
  session->current_loop->looping = true;
  pthread_t * loop_playback_thread = calloc(1, sizeof(pthread_t));
  pthread_create(loop_playback_thread, NULL, loop_playback, session->current_loop);
  pthread_detach(loop_playback_thread);
  
  return NULL;
}
