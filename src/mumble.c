#include <stdio.h>
#include <monome.h>
#include <lo/lo.h> 

#define MONOME_DEVICE "osc.udp://127.0.0.1:8080/monome"

// TODO callbacks
//   - translate event to note mapping
static void button_handler(const monome_event_t *e, void *user_data) {
  int event_x, event_y, event_type;
  
  printf("EVENT!\n");
  // TODO spec this out more
  //      quite possible that midi interaction could be more interesting
  if(event_type == MONOME_BUTTON_DOWN) {
    // TODO play midi note
    //      Midi note on == 0x90
    printf("Button down\n");
    monome_led_on(e->monome, event_x, event_y);
  } else {
    // TODO stop midi note
    //      Midi not off == 0x80
    printf("Button up\n");
    monome_led_off(e->monome, event_x, event_y);
  }
}


int main() {
  // connect to a monome
  // register callbacks
  // 
  monome_t *monome;

  if(!(monome = monome_open(MONOME_DEVICE, "8000"))) {
    printf("Couldn't open monome\n");
    return -1;
  }

  monome_led_all(monome, 0);

  monome_register_handler(monome, MONOME_BUTTON_DOWN, button_handler, NULL);
  monome_register_handler(monome, MONOME_BUTTON_UP, button_handler, NULL);

  monome_event_loop(monome);

  monome_close(monome);
  return 0;
}
