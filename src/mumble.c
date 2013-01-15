#include <stdio.h>
#include <monome.h>
#include <lo/lo.h> 

// TODO callbacks
//   - translate event to note mapping
static void button_handler(const monome_event_t *e, void *user_data) {
  int event_x, event_y, event_type;
  

  // TODO spec this out more
  //      quite possible that midi interaction could be more interesting
  if(event_type == MONOME_BUTTON_DOWN) {
    // TODO play midi note
    //      Midi note on == 0x90
    printf("Button down\n");
  } else {
    // TODO stop midi note
    //      Midi not off == 0x80
    printf("Button up\n");
  }
}


int main() {
  // connect to a monome
  // register callbacks
  // 
  monome_t *monome;
  char *buf;
  asprintf(&buf, "osc.udp://127.0.0.1:%s/%s", "19431", "/mumble");

  monome = monome_open(buf, "8000");
  monome_register_handler(monome, MONOME_BUTTON_DOWN, button_handler, monome);
  monome_register_handler(monome, MONOME_BUTTON_UP, button_handler, monome);

  for(;;) {};

  return 0;
}
