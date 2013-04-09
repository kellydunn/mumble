#include "dispatcher_functions.h"

void play_midi(const monome_event_t *e, void *user_data) {
  unsigned char * midi_data;
  mumble_t *mumble = (mumble_t *) user_data;

  midi_data = midi_data_from_monome_event(midi_data, e, user_data);

  if(e->event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, e->grid.x, e->grid.y);
  }  

  if (mumble->session->recording) { 
    // TODO Implement
    //      - Create midi_event_t
    //      - Append it to current loop
    mumble_midi_event_t * event = calloc(1, sizeof(mumble_midi_event_t));
    event->data = midi_data;

    // TODO timestamp / delay    
    add_midi_event(&mumble->session->current_loop, event);
    printf("recording!");
  }

  
  int midi_fd = (mumble->midi_fd);
  write(midi_fd, midi_data, sizeof(midi_data));
  free(midi_data);
}

void record_midi(const monome_event_t *e, void *user_data) {
  mumble_t *mumble = (mumble_t *) user_data;

  // TODO Somehow refactor with code in #play_midi
  if(e->event_type == MONOME_BUTTON_DOWN) {
    start_recording(mumble->session);
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  } else {
    stop_recording(mumble->session);
    monome_led_off(e->monome, e->grid.x, e->grid.y);
  }
}
