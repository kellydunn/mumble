#include "midi.h"
#include "muxer.h"

// TODO Scope out a bit more.  
//      Seems weird that I need to know about the session in order to create the muxer
mumble_muxer_t * mumble_muxer_init(mumble_muxer_t * muxer) {
  printf("Initializing muxer... ");

  // TODO Refactor into new_mumble_muxer() or some-such
  /* TODO monome_get_rows and monome_get_cols return 0 ?!
  int monome_rows = monome_get_rows(monome);
  int monome_cols = monome_get_cols(monome);
  printf("\n  monome rows detected: %d\n", monome_rows);
  printf("\n  monome cols detected: %d\n", monome_cols);
  */
  int monome_rows = 8;
  int monome_cols = 8;
  muxer = malloc(sizeof(mumble_muxer_t));

  if(muxer == NULL) {
    printf("  Error initializing muxer.");
  }

  muxer->dispatchers = malloc((sizeof(mumble_dispatcher_t) * monome_rows * monome_cols));
  printf("done.\n");

  // TODO Refactor into mumble_mux_t and introduce
  //      A configuration file that can be customized
  //      that contains a default mux
  int x, y;

  printf("Attaching callbacks...");
  for(x = 0; x < monome_rows; x++) {
    for(y = 0; y < monome_cols; y++) {
      printf("  initializing: [%d, %d]\n",x, y);
      mumble_dispatcher_t *dispatcher;
      dispatcher = malloc(sizeof(mumble_dispatcher_t));
      dispatcher->dispatch_func = play_midi;
      muxer->dispatchers[(y*8) +x] = *dispatcher;
      if(&(muxer->dispatchers[(y * 8) + x]) == NULL) {
        printf("  Couldn't attach callback\n");
      }
    }
  }
  printf("done.\n");

  return muxer;
}
