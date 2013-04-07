#include "stdlib.h"
#include "stdio.h"
#include <monome.h>
//#include "midi.h"

typedef void (*dispatch_func)(const monome_event_t *e, void *user_data);

typedef struct _mumble_dispatcher_t {
  dispatch_func (*dispatch_func)(const monome_event_t *e, void *user_data);
} mumble_dispatcher_t;

typedef struct _mumble_muxer_t {
  mumble_dispatcher_t* dispatchers;
} mumble_muxer_t;
