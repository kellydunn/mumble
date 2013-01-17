#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <monome.h>

// TODO Read port from serialoscd
#define MONOME_DEVICE "osc.udp://127.0.0.1:13437/monome"

// TODO Research midi device daemon?
#define MIDI_DEVICE "/dev/midi"
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

// ROADMAP Somehow account for custom scales
#define LOW_C 36

// TODO Handle note velocity more elegantly
#define VELOCITY 127 

typedef void (*dispatch_func) (const monome_event_t *e, void *user_data);

typedef struct _mumble_dispatcher_t {
  dispatch_func (*dispatch_func)();
} mumble_dispatcher_t;

typedef struct _mumble_muxer_t {
  int midi_fd; // TODO Determine if needed, or should be designed better
  mumble_dispatcher_t* dispatchers;
} mumble_muxer_t;

static void button_handler(const monome_event_t *e, void *user_data) { 
  mumble_muxer_t * muxer = (mumble_muxer_t*) user_data;

  // TODO Would be good to refactor this out from muxer
  //      and dispatched functions as well
  int event_x, event_y, event_type;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  int rows = monome_get_rows(e->monome);
  int cols = monome_get_cols(e->monome);

  mumble_dispatcher_t * dispatcher = &(muxer->dispatchers[((event_y + 1) * rows) + event_x]);
  dispatcher->dispatch_func(e, user_data);
}

// TODO Reorganize out into midi_funcs.c or something
void play_midi(const monome_event_t *e, void *user_data) {

  // TODO Would be good to refactor this out from muxer
  //      and dispatched functions as well
  int event_x, event_y, event_type;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  unsigned char midi_data[3];
  int midi_note = ((LOW_C * (event_y + 1)) + event_x);

  midi_data[1] = midi_note;
  midi_data[2] = VELOCITY;

  if(event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, event_x, event_y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, event_x, event_y);
  }  
  
  int midi_fd = ((mumble_muxer_t *)user_data)->midi_fd;
  write(midi_fd, midi_data, sizeof(midi_data));
}

int main() {
  monome_t *monome;
  mumble_muxer_t *muxer;
  
  if(!(monome = monome_open(MONOME_DEVICE, "8000"))) {
    printf("Couldn't open monome\n");
    return -1;
  }

  // TODO Refactor into new_mumble_muxer() or some-such
  int monome_rows = monome_get_rows(monome);
  int monome_cols = monome_get_cols(monome);
  muxer = malloc(sizeof(muxer) * monome_rows * monome_cols);

  // TODO Control functions
  //      For now, everything plays a midi note
  int x, y;
  for(x = 0; x < monome_rows; x++) {
    for(y = 0; y < monome_cols; y++) {
      mumble_dispatcher_t *dispatcher;
      dispatcher = malloc(sizeof(mumble_dispatcher_t));
      dispatcher->dispatch_func = play_midi;
      *(muxer->dispatchers++) = *dispatcher;
    }
  }

  // TODO handle more gracefully
  muxer->midi_fd = open(MIDI_DEVICE, O_WRONLY, 0);
  if (muxer->midi_fd < 0) {
    printf("Error:  Could not open %s\n", MIDI_DEVICE);
    return 1;
  }

  monome_register_handler(monome, MONOME_BUTTON_DOWN, button_handler, (void *)muxer);
  monome_register_handler(monome, MONOME_BUTTON_UP, button_handler, (void *)muxer);

  monome_event_loop(monome);

  monome_close(monome);
  close(muxer->midi_fd);

  return 0;
}
