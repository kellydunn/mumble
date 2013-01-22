#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <monome.h>
#include "mumble/muxer.h"

// ROADMAP Somehow account for custom scales
#define LOW_C 36

// TODO Handle note velocity more elegantly
#define VELOCITY 127 

typedef struct _mumble_t {
  int midi_fd;
  monome_t * monome;
  mumble_muxer_t * muxer;
  int root;
  int velocity;
} mumble_t;

static void button_handler(const monome_event_t *e, void *user_data);
int mumble_init(mumble_t* mumble);
