#include "../mumble.h"
#include "midi.h"

// Determines the corresponding MIDI note and event from the monome button event
// and writes it to the MIDI device file descriptor.
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
  
  int midi_fd = (mumble->midi_fd);
  write(midi_fd, midi_data, sizeof(midi_data));
  free(midi_data);
}

void record_midi(const monome_event_t *e, void *user_data) {
  // TODO Implement
}

void playback_midi(const monome_event_t *e, void *user_data) {
  // TODO Implement  
}

// Generates a midi message from a monome event.
unsigned char * midi_data_from_monome_event(unsigned char * midi_data, const monome_event_t * e, void *user_data) {
  int midi_note_message_len = 3;
  midi_data = (unsigned char *)calloc(midi_note_message_len, sizeof(unsigned char));

  int event_x, event_y, event_type;
  mumble_t *mumble = (mumble_t *) user_data;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;  

  int midi_note = ((mumble->root * (event_y + 1)) + event_x);  
  midi_data[1] = midi_note;
  midi_data[2] = mumble->velocity;  

  return midi_data;
}
