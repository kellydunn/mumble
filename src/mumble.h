#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <monome.h>
#include "mumble/muxer.h"

typedef struct _mumble_t {
  monome_t * monome;
  mumble_muxer_t * muxer;
} mumble_t;

static void button_handler(const monome_event_t *e, void *user_data);
void mumble_init(mumble_t* mumble);
