#include "muxer.h"
#include "dispatcher_functions.h"

// @return {mumble_muxer_t*} muxer.  A newly initialized muxer made with the default muxer, 
//         or NULL if no memory can be allocated.
mumble_muxer_t * new_muxer() {
  mumble_muxer_t * muxer = calloc(1, sizeof(mumble_muxer_t));

  if(muxer == NULL) {
    printf("[new_muxer] Error initializing muxer.");
    return NULL;
  }

  muxer->dispatchers = malloc((sizeof(mumble_dispatcher_t) * MONOME_ROWS * MONOME_COLS));

  // TODO Refactor into mumble_mux_t and introduce
  //      A configuration file that can be customized
  //      that contains a default mux

  // Attach the start_recording method to the 4th button in the first row
  int x, y;

  for(y = 0; y < MONOME_ROWS; y++) {
    for(x = 0; x < MONOME_COLS; x++) {
      if(y == 0) {
        handle_func(muxer, x, record_midi);
      } else {
        handle_func(muxer, ((y * MONOME_ROWS) + x), play_midi);
      }

      if(&(muxer->dispatchers[(y * MONOME_ROWS) + x]) == NULL) {
        printf("  Couldn't attach callback for [%d, %d].\n", x, y);
      }
    }
  }

  return muxer;
}

// @param {mumble_muxer_t*} muxer.  The muxer in which to attach the dispatch functions.
// @param {int} monome_position.  The specific monome_postion, represented as a positive integer that correlates to a position on the monome grid.
// @param {dispatch_func} func.  The {dispatch_func} in which to attach button events for the paticular monome_position
void handle_func(mumble_muxer_t * muxer, int monome_position, dispatch_func func) {
  mumble_dispatcher_t * dispatcher;
  dispatcher = malloc(sizeof(mumble_dispatcher_t));
  dispatcher->dispatch_func = func;
  muxer->dispatchers[monome_position] = *dispatcher;
}
