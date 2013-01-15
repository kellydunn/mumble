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
  } else {
    // TODO stop midi note
    //      Midi not off == 0x80
  }
}


int main() {
  // connect to a monome
  // register callbacks
  // 
  return 0;
}
