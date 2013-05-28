#include <monome.h>
#include "mumble/mumble.h"

// This is the main entry point for mumble.
// glhf n____n
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
