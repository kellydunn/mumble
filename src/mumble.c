#include <monome.h>
#include "mumble.h"

// TODO Read port from serialoscd
#define MONOME_DEVICE "osc.udp://127.0.0.1:19043/monome"
//#define MONOME_DEVICE "osc.udp://127.0.0.1:10383/monome"

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

  char * home = getenv("HOME");
  char file_path_buf[256];  //sprintf you so nasty
  sprintf(file_path_buf, "%s/.mumble/config.yml", home);

  mumble_muxer_t * muxer = (mumble_muxer_t *) mumble_muxer_init();
  mumble_session_t * session = (mumble_session_t *) mumble_session_init(mumble);
  mumble_config_t * config = (mumble_config_t *) new_config(file_path_buf);
  
  monome = monome_open(config->monome_path, "8000");

  // TODO handle more gracefully
  printf("Opening midi device...");
  mumble->midi_fd = open(config->midi_device, O_WRONLY, 0);
  if (mumble->midi_fd < 0) {
    printf("Error:  Could not open %s\n", config->midi_device);
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

int eyes [8][2] = {
  {1, 1},
  {1, 2},
  {2, 1},
  {2, 2},
  {5, 1},
  {5, 2},
  {6, 1},
  {6, 2}
};

int mouth_row_1 [8][2] = {
  {0, 4},
  {1, 4},
  {2, 4},
  {3, 4},
  {4, 4},
  {5, 4},
  {6, 4},
  {7, 4}
};

int mouth_row_2 [6][2] = {
  {1, 5},
  {2, 5},
  {3, 5},
  {4, 5},
  {5, 5},
  {6, 5},
};

int mouth_row_3 [4][2] = {
  {2, 6},
  {3, 6},
  {4, 6},
  {5, 6},
};

// TODO have some generiec face_part_on / face_part_off method
void mumble_intro_eyes_off(mumble_t * mumble) {
  int i;
  for(i=0; i<8; i++) {
    monome_led_off(mumble->monome, eyes[i][0], eyes[i][1]);
  }
}

void mumble_intro_eyes_on(mumble_t * mumble) {
  int i;
  for(i=0; i<8; i++) {
    monome_led_on(mumble->monome, eyes[i][0], eyes[i][1]);
  }
}

void mumble_intro_mouth_row_1_on(mumble_t * mumble) {
  int i;
  for(i=0; i<8; i++) {
    monome_led_on(mumble->monome, mouth_row_1[i][0], mouth_row_1[i][1]);
  }
}

void mumble_intro_mouth_row_1_off(mumble_t * mumble) {
  int i;
  for(i=0; i<8; i++) {
    monome_led_off(mumble->monome, mouth_row_1[i][0], mouth_row_1[i][1]);
  }
}

void mumble_intro_mouth_row_2_on(mumble_t * mumble) {
  int i;
  for(i=0; i<6; i++) {
    monome_led_on(mumble->monome, mouth_row_2[i][0], mouth_row_2[i][1]);
  }
}

void mumble_intro_mouth_row_2_off(mumble_t * mumble) {
  int i;
  for(i=0; i<6; i++) {
    monome_led_off(mumble->monome, mouth_row_2[i][0], mouth_row_2[i][1]);
  }
}

void mumble_intro_mouth_row_3_on(mumble_t * mumble) {
  int i;
  for(i=0; i<4; i++) {
    monome_led_on(mumble->monome, mouth_row_3[i][0], mouth_row_3[i][1]);
  }
}

void mumble_intro_mouth_row_3_off(mumble_t * mumble) {
  int i;
  for(i=0; i<4; i++) {
    monome_led_off(mumble->monome, mouth_row_3[i][0], mouth_row_3[i][1]);
  }
}

void mumble_intro_blink(mumble_t * mumble) {
 // Blinky
  usleep(100000);
  mumble_intro_eyes_off(mumble);

  // Blinky
  usleep(75000);
  mumble_intro_eyes_on(mumble);
}

void mumble_intro_talk(mumble_t * mumble) {
 // :]
  mumble_intro_mouth_row_2_off(mumble);
  mumble_intro_mouth_row_3_off(mumble);

  usleep(70000);
  mumble_intro_mouth_row_2_on(mumble);
  mumble_intro_mouth_row_3_on(mumble);
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
  mumble_intro_eyes_on(mumble);
  mumble_intro_mouth_row_1_on(mumble);

  // :]
  usleep(90000);
  mumble_intro_mouth_row_2_on(mumble);

  // :D
  usleep(80000);
  mumble_intro_mouth_row_3_on(mumble);

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

  mumble = mumble_init(mumble);

  monome_register_handler(mumble->monome, MONOME_BUTTON_DOWN, button_handler, (void *)mumble);
  monome_register_handler(mumble->monome, MONOME_BUTTON_UP, button_handler, (void *)mumble);

  mumble_intro(mumble);

  monome_event_loop(mumble->monome);

  monome_close(mumble->monome);
  close(mumble->midi_fd);

  return 0;
}

// Generates a midi message from a monome event.
// TODO Debug, apparently this stuff generates segfaults when being ran in the loop playback thread. 
//      Maybe mumble_midi_event_t structs can hold this data inside of them, and be read later.
unsigned char * midi_data_from_monome_event(unsigned char * midi_data, const monome_event_t * e, void *user_data) {
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

  return midi_data;
}

void mumble_write_to_midi_device(mumble_t * mumble, unsigned char * midi_data) {
  int midi_fd = (mumble->midi_fd);
  write(midi_fd, midi_data, sizeof(midi_data));
}
