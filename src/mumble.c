#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <monome.h>
#include <lo/lo.h> 

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

static void button_handler(const monome_event_t *e, void *user_data) {
  int event_x, event_y, event_type;

  event_x = e->grid.x;
  event_y = e->grid.y;
  event_type = e->event_type;

  unsigned char midi_data[3];
  int midi_note = ((LOW_C * (event_y + 1)) + event_x);

  midi_data[1] = midi_note;
  midi_data[2] = VELOCITY;

  // TODO spec this out more
  //      quite possible that midi interaction could be more interesting
  if(event_type == MONOME_BUTTON_DOWN) {
    midi_data[0] = MIDI_NOTE_ON;
    monome_led_on(e->monome, event_x, event_y);
  } else {
    midi_data[0] = MIDI_NOTE_OFF;
    monome_led_off(e->monome, event_x, event_y);
  }  

  int midi_fd = (int) &user_data;
  write(midi_fd, midi_data, sizeof(midi_data));
}

int main() {
  monome_t *monome;

  if(!(monome = monome_open(MONOME_DEVICE, "8000"))) {
    printf("Couldn't open monome\n");
    return -1;
  }

  // TODO handle more gracefully
  int midi_fd = open(MIDI_DEVICE, O_WRONLY, 0);
  if (midi_fd < 0) {
    printf("Error:  Could not open %s\n", MIDI_DEVICE);
    return 1;
  }

  monome_register_handler(monome, MONOME_BUTTON_DOWN, button_handler, (void *)midi_fd);
  monome_register_handler(monome, MONOME_BUTTON_UP, button_handler, (void *)midi_fd);

  monome_event_loop(monome);

  monome_close(monome);
  close(midi_fd);

  return 0;
}
