#include "stdio.h"
#include "stdlib.h"
#include "loop.h"
#include "time.h"

mumble_loop_t * new_loop(mumble_session_t * session) {
  mumble_loop_t * loop = calloc(1, sizeof(mumble_loop_t));
  loop->looping = false;
  loop->inst = 0; // TODO grab from mumble session
  loop->events = new_list();
  loop->session = session;
}

// Adds a midi event
void add_midi_event(mumble_loop_t * loop, mumble_midi_event_t * event) {
  printf("Adding midi event!\n");
  printf("  [event_type:%d, event_x:%d, event_y%d]!\n", event->monome_event->event_type, event->monome_event->grid.x, event->monome_event->grid.y);
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

void loop_playback_midi_event(monome_event_t * e, void * user_data) {
  // TODO Refactor with play_midi in dispatch_functions.c
  
  unsigned char * midi_data;
  mumble_t * mumble = (mumble_t *) user_data;

  //midi_data = (unsigned char *) midi_data_from_monome_event(midi_data, e, user_data);

  //monome_led_on(e->monome, e->grid.x, e->grid.y);
  //usleep(1000);
  //monome_led_off(e->monome, e->grid.x, e->grid.y);

  //printf("playback event [event_type:%d, event_x: %d, event_y:%d]\n", e->event_type, e->grid.x, e->grid.y);
  monome_led_on(e->monome, e->grid.x, e->grid.y);
  usleep(1000);
  monome_led_off(e->monome, e->grid.x, e->grid.y);
  usleep(1000);

  if(e->event_type == MONOME_BUTTON_DOWN) {
    //midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, e->grid.x, e->grid.y);
  } else {
    //midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, e->grid.x, e->grid.y);
  }  
  
  //mumble_write_to_midi_device(mumble, midi_data);

  free(midi_data);  
}
