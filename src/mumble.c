#include "mumble/midi.h"
#include "mumble.h"
#include <monome.h>

// TODO Read port from serialoscd
#define MONOME_DEVICE "osc.udp://127.0.0.1:15291/monome"

static void button_handler(const monome_event_t *e, void *user_data) { 
  printf("Handler called:");
  mumble_t *mumble = (mumble_t*) user_data;

  if(&(mumble->muxer->dispatchers[0]) == NULL) {
    printf("\nWHAT\n\n");
  }

  // TODO Would be good to refactor this out from muxer
  //      and dispatched functions as well
  int event_x, event_y, event_type;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  int rows = monome_get_rows(e->monome);
  int cols = monome_get_cols(e->monome);

  printf("Dispatching event: [%d, %d]\n", event_x, event_y);
  
  mumble_dispatcher_t * dispatcher = &(mumble->muxer->dispatchers[((event_y + 1) * rows) + event_x]);

  if(dispatcher == NULL) {
    printf("  Error finding associated callback.\n");
  }

  if(dispatcher->dispatch_func == NULL) {
    printf("  Error finding associated callback function.\n");
  } else {
    printf("  We cool.\n");
  }

  dispatcher->dispatch_func(e, user_data);
}

// SPEC 
//   - initialize monome
//   - register callbacks
//   - open midi device
// TODO
//   - Error handling
mumble_t * mumble_init(mumble_t* mumble) {
  monome_t *monome;
  mumble_muxer_t *muxer;

  printf("Initializing mumble...");
  mumble = malloc(sizeof(mumble_t));
  printf("done.\n");

  printf("Opening monome...");
  monome = monome_open(MONOME_DEVICE, "8000");
  printf("done.\n");

  // TODO Refactor into new_mumble_muxer() or some-such
  /* TODO monome_get_rows and monome_get_cols return 0 ?!
  int monome_rows = monome_get_rows(monome);
  int monome_cols = monome_get_cols(monome);
  printf("\n  monome rows detected: %d\n", monome_rows);
  printf("\n  monome cols detected: %d\n", monome_cols);
  */
  printf("Initializing muxer... ");
  int monome_rows = 8;
  int monome_cols = 8;
  muxer = malloc(sizeof(mumble_muxer_t));

  if(muxer == NULL) {
    printf("  Error initializing muxer.");
  }

  muxer->dispatchers = malloc(sizeof(mumble_dispatcher_t) * monome_rows * monome_cols);

  printf("done.\n");
  // TODO Refactor into mumble_mux_t and introduce
  //      A configuration file that can be customized
  //      that contains a default mux
  int x, y;

  printf("Attaching callbacks...");
  for(x = 0; x < monome_rows; x++) {
    printf("  row: %d\n", x);
    for(y = 0; y < monome_cols; y++) {
      printf("  col: %d\n",y);
      mumble_dispatcher_t *dispatcher;
      dispatcher = malloc(sizeof(mumble_dispatcher_t));
      dispatcher->dispatch_func = play_midi;
      *(muxer->dispatchers++) = *dispatcher;
    }
  }
  printf("done.\n");

  // TODO handle more gracefully
  printf("Opening midi device...");
  mumble->midi_fd = open(MIDI_DEVICE, O_WRONLY, 0);
  if (mumble->midi_fd < 0) {
    printf("Error:  Could not open %s\n", MIDI_DEVICE);
    return NULL;
  }
  printf("done.\n");

  printf("Attaching monome to session...");
  mumble->monome = monome;
  if(&mumble->monome == NULL) {
    printf("Error: Could not attach monome.\n");
    return NULL;
  }
  printf("done.\n");

  printf("Attaching muxer to session...");
  mumble->muxer = muxer;
  printf("done.\n");

  printf("Setting session root note...");
  mumble->root = LOW_C;
  printf("done.\n");

  printf("Setting session root velocity...");
  mumble->velocity = VELOCITY;
  printf("done.\n");

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
