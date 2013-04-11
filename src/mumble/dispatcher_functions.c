#include "stdlib.h"
#include "stdio.h"
#include "dispatcher_functions.h"
#include "time.h"

// Why is this not in time.h ? -____________-;;
struct timeval timeval_subtract(struct timeval t0, struct timeval t1) {
  struct timeval result;
  
  if (t1.tv_usec < t0.tv_usec) {
    int nsec = (t0.tv_usec - t1.tv_usec) / 1000000 + 1;
    t0.tv_usec -= 1000000 * nsec;
    t0.tv_sec += nsec;
  }

  if (t1.tv_usec - t0.tv_usec > 1000000) {
    int nsec = (t1.tv_usec - t0.tv_usec) / 1000000;
    t0.tv_usec += 1000000 * nsec;
    t0.tv_sec -= nsec;
  }
  
  // Compute the time remaining to wait.  tv_usec is certainly positive
  result.tv_sec = t1.tv_sec - t0.tv_sec;
  result.tv_usec = t1.tv_usec - t0.tv_usec;

  return result;
}

void play_midi(const monome_event_t *e, void *user_data) {
  unsigned char * midi_data;
  mumble_t * mumble = (mumble_t *) user_data;

  midi_data = (unsigned char *) midi_data_from_monome_event(midi_data, e, user_data);

  if(e->event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, e->grid.x, e->grid.y);
  }  

  if (mumble->session->recording == true) { 
    mumble_midi_event_t * event = calloc(1, sizeof(mumble_midi_event_t));

    struct timeval now;
    gettimeofday(&now, 0);

    event->timestamp = now;

    if(!is_empty(mumble->session->current_loop->events)) {
      mumble_midi_event_t * last_event = (mumble_midi_event_t *) mumble->session->current_loop->events->tail->data;
      struct timeval diff;
      diff =  timeval_subtract(last_event->timestamp, now);
      last_event->delay = diff.tv_usec;
      printf("Delay of %d detected\n",last_event->delay);

      // Add to total duration of the loop so we may account for it 
      // When closing the event chain.
      mumble->session->current_loop->duration += diff.tv_usec;
    }

    // Deep copy of monome information
    // TODO Make this less suck-tastic
    event->monome_event = calloc(1, sizeof(monome_event_t));
    event->monome_event->event_type = e->event_type;
    event->monome_event->grid.x = e->grid.x;
    event->monome_event->grid.y = e->grid.y;
    event->monome_event->monome = e->monome;

    event->data = midi_data;
    
    // TODO timestamp / delay    
    add_midi_event(mumble->session->current_loop, event);
  }
  
  mumble_write_to_midi_device(mumble, midi_data);
  free(midi_data);
}

void record_midi(const monome_event_t *e, void *user_data) {
  mumble_t *mumble = (mumble_t *) user_data;

  // TODO Somehow refactor with code in #play_midi

  if(e->event_type == MONOME_BUTTON_DOWN) {
    start_recording(mumble->session);
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  }
}
