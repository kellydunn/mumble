#include "muxer.h"
#include "dispatcher_functions.h"

// TODO Dynamically determine size of monome
#define MONOME_ROWS 8
#define MONOME_COLS 8

// TODO Scope out a bit more.  
//      Seems weird that I need to know about the session in order to create the muxer
mumble_muxer_t * mumble_muxer_init() {
  printf("Initializing muxer... ");
  mumble_muxer_t * muxer = calloc(1, sizeof(mumble_muxer_t));

  if(muxer == NULL) {
    printf("  Error initializing muxer.");
  }

  muxer->dispatchers = malloc((sizeof(mumble_dispatcher_t) * MONOME_ROWS * MONOME_COLS));
  printf("done.\n");

  // TODO Refactor into mumble_mux_t and introduce
  //      A configuration file that can be customized
  //      that contains a default mux

  // Attach the start_recording method to the 4th button in the first row

  int x, y;
  printf("Attaching callbacks...");
  for(y = 0; y < MONOME_ROWS; y++) {
    for(x = 0; x < MONOME_COLS; x++) {
      if(y == 0) {
        handle_func(muxer, x, record_midi);
      } else {
        handle_func(muxer, ((y * MONOME_ROWS) + x), play_midi);
      }

      if(&(muxer->dispatchers[(y * MONOME_ROWS) + x]) == NULL) {
        printf("  Couldn't attach callback\n");
      }
    }
  }

  printf("done.\n");

  return muxer;
}

// Attaches a mumble dispatch_function to a specific button position on the monome
void handle_func(mumble_muxer_t * muxer, int monome_position, dispatch_func func) {
  mumble_dispatcher_t * dispatcher;
  dispatcher = malloc(sizeof(mumble_dispatcher_t));
  dispatcher->dispatch_func = func;
  muxer->dispatchers[monome_position] = *dispatcher;
}
