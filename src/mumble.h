#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <monome.h>
#include "types.h"

// ROADMAP Somehow account for custom scales
#define LOW_C 36

// TODO Handle note velocity more elegantly
#define VELOCITY 127 

// ROADMAP Provide bpm configuration
#define BPM 120.0

static void button_handler(const monome_event_t *e, void *user_data);
mumble_t * mumble_init(mumble_t* mumble);
unsigned char * midi_data_from_monome_event(unsigned char * midi_data, const monome_event_t *e, void *user_data);


