#include "../mumble.h"
#include "midi.h"

void play_midi(const monome_event_t *e, void *user_data) {

  // TODO Would be good to refactor this out from muxer
  //      and dispatched functions as well
  int event_x, event_y, event_type;
  mumble_t *mumble = (mumble_t *) user_data;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  unsigned char midi_data[3];
  int midi_note = ((mumble->root * (event_y + 1)) + event_x);

  midi_data[1] = midi_note;
  midi_data[2] = mumble->velocity;

  if(event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, event_x, event_y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, event_x, event_y);
  }  
  
  int midi_fd = (mumble->midi_fd);
  write(midi_fd, midi_data, sizeof(midi_data));
}
