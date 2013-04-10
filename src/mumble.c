#include <monome.h>
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

void mumble_intro_blink(mumble_t * mumble) {
 // Blinky
  usleep(100000);
  monome_led_off(mumble->monome, 1, 1);
  monome_led_off(mumble->monome, 1, 2);
  monome_led_off(mumble->monome, 2, 1);
  monome_led_off(mumble->monome, 2, 2);

  monome_led_off(mumble->monome, 1, 6);
  monome_led_off(mumble->monome, 1, 5);
  monome_led_off(mumble->monome, 2, 6);
  monome_led_off(mumble->monome, 2, 5);

  // Blinky
  usleep(75000);
  monome_led_on(mumble->monome, 1, 1);
  monome_led_on(mumble->monome, 1, 2);
  monome_led_on(mumble->monome, 2, 1);
  monome_led_on(mumble->monome, 2, 2);

  monome_led_on(mumble->monome, 1, 6);
  monome_led_on(mumble->monome, 1, 5);
  monome_led_on(mumble->monome, 2, 6);
  monome_led_on(mumble->monome, 2, 5);
}

void mumble_intro_talk(mumble_t * mumble) {
 // :]
  monome_led_off(mumble->monome, 5, 1);
  monome_led_off(mumble->monome, 5, 2);
  monome_led_off(mumble->monome, 5, 3);
  monome_led_off(mumble->monome, 5, 4);
  monome_led_off(mumble->monome, 5, 5);
  monome_led_off(mumble->monome, 5, 6);
  monome_led_off(mumble->monome, 6, 2);
  monome_led_off(mumble->monome, 6, 3);
  monome_led_off(mumble->monome, 6, 4);
  monome_led_off(mumble->monome, 6, 5);

  usleep(70000);
  monome_led_on(mumble->monome, 5, 1);
  monome_led_on(mumble->monome, 5, 2);
  monome_led_on(mumble->monome, 5, 3);
  monome_led_on(mumble->monome, 5, 4);
  monome_led_on(mumble->monome, 5, 5);
  monome_led_on(mumble->monome, 5, 6);
  monome_led_on(mumble->monome, 6, 2);
  monome_led_on(mumble->monome, 6, 3);
  monome_led_on(mumble->monome, 6, 4);
  monome_led_on(mumble->monome, 6, 5);
}

// :D
void mumble_intro(mumble_t * mumble) {
  int x, y;

  for(x = 0; x < 8; x++) {
    for(y = 0; y < 8; y++) {
      monome_led_off(mumble->monome, x, y);
    }
  }

  // :|
  monome_led_on(mumble->monome, 1, 1);
  monome_led_on(mumble->monome, 1, 2);
  monome_led_on(mumble->monome, 2, 1);
  monome_led_on(mumble->monome, 2, 2);

  monome_led_on(mumble->monome, 1, 6);
  monome_led_on(mumble->monome, 1, 5);
  monome_led_on(mumble->monome, 2, 6);
  monome_led_on(mumble->monome, 2, 5);

  monome_led_on(mumble->monome, 4, 0);
  monome_led_on(mumble->monome, 4, 1);
  monome_led_on(mumble->monome, 4, 2);
  monome_led_on(mumble->monome, 4, 3);
  monome_led_on(mumble->monome, 4, 4);
  monome_led_on(mumble->monome, 4, 5);
  monome_led_on(mumble->monome, 4, 6);
  monome_led_on(mumble->monome, 4, 7);

  // :]
  usleep(90000);
  monome_led_on(mumble->monome, 5, 1);
  monome_led_on(mumble->monome, 5, 2);
  monome_led_on(mumble->monome, 5, 3);
  monome_led_on(mumble->monome, 5, 4);
  monome_led_on(mumble->monome, 5, 5);
  monome_led_on(mumble->monome, 5, 6);

  // :D
  usleep(80000);
  monome_led_on(mumble->monome, 6, 2);
  monome_led_on(mumble->monome, 6, 3);
  monome_led_on(mumble->monome, 6, 4);
  monome_led_on(mumble->monome, 6, 5);

  mumble_intro_blink(mumble);
  usleep(120000);
  mumble_intro_talk(mumble);
  usleep(120000);
  mumble_intro_talk(mumble);
  usleep(200000);

  // Fade
  int bcount;
  for(bcount = 15; bcount > 0; bcount --) {
    usleep(75000);
    monome_led_intensity(mumble->monome, bcount);
  }

  // OFF
  for(x = 0; x < 8; x++) {
    for(y = 0; y < 8; y++) {
      monome_led_off(mumble->monome, x, y);
    }
  }

  // Back to normal
  monome_led_intensity(mumble->monome, 0xF);
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

  printf("Mumble online :)\n");
  mumble_intro(mumble);

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
