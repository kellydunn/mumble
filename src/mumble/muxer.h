#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <monome.h>

typedef void (*dispatch_func) (const monome_event_t *e, void *user_data);

typedef struct _mumble_dispatcher_t {
  dispatch_func (*dispatch_func)(const monome_event_t *e, void *user_data);
} mumble_dispatcher_t;

typedef struct _mumble_muxer_t {
  int midi_fd; // TODO Determine if needed, or should be designed better
  mumble_dispatcher_t* dispatchers;
} mumble_muxer_t;
