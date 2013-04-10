#include "mumble.h"

// TODO Read port from serialoscd
#define MONOME_DEVICE "osc.udp://127.0.0.1:19043/monome"

static void button_handler(const monome_event_t *e, void *user_data) { 
  printf("Handler called:");
  mumble_t *mumble = (mumble_t*) user_data;

  // TODO Would be good to refactor this out from muxer
  //      and dispatched functions as well
  int event_x, event_y, event_type;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  //int rows = monome_get_rows(e->monome);
  //int cols = monome_get_cols(e->monome);

  int rows = 8;
  int cols = 8;

  printf("Dispatching event: [%d, %d] to callback: %d\n", event_x, event_y, ((event_y * rows) + (event_x)));  
  mumble_dispatcher_t * dispatcher = &(mumble->muxer->dispatchers[((event_y) * rows-1) + event_x]);

  if(dispatcher == NULL) {
    printf("  Error finding associated callback.\n");
  }

  if(dispatcher->dispatch_func == NULL) {
    printf("  Error finding associated callback function.\n");
  } 

  dispatcher->dispatch_func(e, user_data);
}

// Initializes a mumble session 
mumble_t * mumble_init(mumble_t* mumble) {
  monome_t *monome;

  mumble = calloc(1, sizeof(mumble_t));
  monome = monome_open(MONOME_DEVICE, "8000");

  mumble_muxer_t * muxer = (mumble_muxer_t *) mumble_muxer_init();
  mumble_session_t * session = (mumble_session_t *) mumble_session_init(mumble);

  // TODO handle more gracefully
  printf("Opening midi device...");
  mumble->midi_fd = open(MIDI_DEVICE, O_WRONLY, 0);
  if (mumble->midi_fd < 0) {
    printf("Error:  Could not open %s\n", MIDI_DEVICE);
    return NULL;
  }
  printf("done.\n");

  mumble->monome = monome;
  if(&mumble->monome == NULL) {
    return NULL;
  }

  mumble->muxer = muxer;
  mumble->session = session;
  mumble->root = LOW_C;
  mumble->velocity = VELOCITY;
  mumble->bpm = BPM;

  return mumble;
}

int main() {
  mumble_t * mumble;
  int err;

  printf("Creating session...\n");
  mumble = mumble_init(mumble);
  printf("Finished creating session.\n");

  if(mumble->monome == NULL) {
    printf("no way");
  }

  printf("Registering handlers...");
  monome_register_handler(mumble->monome, MONOME_BUTTON_DOWN, button_handler, (void *)mumble);
  monome_register_handler(mumble->monome, MONOME_BUTTON_UP, button_handler, (void *)mumble);
  printf("done.\n");

  monome_event_loop(mumble->monome);

  monome_close(mumble->monome);
  close(mumble->midi_fd);

  return 0;
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

void mumble_write_to_midi_device(mumble_t * mumble, unsigned char * midi_data) {
  int midi_fd = (mumble->midi_fd);
  write(midi_fd, midi_data, sizeof(midi_data));
}
