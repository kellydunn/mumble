// This represents a single loop in a mumble session.
// A loop contains a collection of events at specfic times and is able to replay them in the order they were recorded.
// A loop plays for a specific amount of time, then starts at the begining of its even list and replays itself indefinitely.

#include "stdio.h"
#include "stdlib.h"
#include "loop.h"
#include "time.h"
#include "list.h"
#include "mumble.h"

// @return {mumble_loop_t*} A new mumble loop.
mumble_loop_t * new_loop(mumble_session_t * session) {
  mumble_loop_t * loop = calloc(1, sizeof(mumble_loop_t));
  loop->looping = false;
  loop->inst = 0; // TODO grab from mumble session
  loop->events = (mumble_list_t *) new_list();
  loop->session = session;
  loop->duration = 0;
  return loop;
}

// Adds a midi event
void add_midi_event(mumble_loop_t * loop, mumble_midi_event_t * event) {
  list_append(loop->events, event);
}

// Playsback the actual MIDI events recorded from earlier
void * loop_playback(void * args) {
  mumble_loop_t * loop = (mumble_loop_t *) args;
  mumble_list_node_t * current = calloc(1, sizeof(mumble_list_node_t));
  current = loop->events->head;

  for(;;) {

    mumble_midi_event_t * current_midi_event = (mumble_midi_event_t *) current->data;
    loop_playback_midi_event(current_midi_event->monome_event, loop->session->mumble);
    usleep(current_midi_event->delay);
    current = current->next;

    if(!loop->looping) {
      break;
    }
  }

  return NULL;
}

// TODO nomenclature is rather bad here, lets rename this to better fit the api direction
void loop_playback_midi_event(const monome_event_t * e, void * user_data) {
  // TODO Refactor with play_midi in dispatch_functions.c
  
  unsigned char * midi_data;

  //midi_data = (unsigned char *) midi_data_from_monome_event(midi_data, e, user_data);
  int midi_note_message_len = 3;
  midi_data = (unsigned char *) calloc(midi_note_message_len, sizeof(unsigned char));

  int event_x, event_y, event_type;
  mumble_t *mumble = (mumble_t *) user_data;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;  

  int midi_note = ((mumble->root * (event_y + 1)) + event_x);  
  midi_data[1] = midi_note;
  midi_data[2] = mumble->velocity;  

  if(e->event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, e->grid.x, e->grid.y);
  }  
  
  mumble_write_to_midi_device(mumble, midi_data);
}
