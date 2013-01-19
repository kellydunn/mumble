#include "mumble.h"
#include "mumble/midi.h"

// TODO Read port from serialoscd
#define MONOME_DEVICE "osc.udp://127.0.0.1:13437/monome"

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

// SPEC 
//   - initialize monome
//   - register callbacks
//   - open midi device
// TODO
//   - Error handling
void mumble_init(mumble_t* mumble) {
  monome_t *monome;
  mumble_muxer_t *muxer;

  mumble = malloc(sizeof(mumble_t));
  
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

  mumble->monome = monome;
  mumble->muxer = muxer;
}

int main() {
  mumble_t * mumble;
  mumble_init(mumble);

  monome_register_handler(mumble->monome, MONOME_BUTTON_DOWN, button_handler, (void *)mumble->muxer);
  monome_register_handler(mumble->monome, MONOME_BUTTON_UP, button_handler, (void *)mumble->muxer);

  monome_event_loop(mumble->monome);

  monome_close(mumble->monome);
  close(mumble->muxer->midi_fd);

  return 0;
}
