#ifndef _MUMBLE_H
#define _MUMBLE_H

#include <linux/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <monome.h>

#include "../types.h"
#include "muxer.h"
#include "session.h"
#include "config.h"

// ROADMAP Somehow account for custom scales
#define LOW_C 24

// TODO Handle note velocity more elegantly
#define VELOCITY 127 

// ROADMAP Provide bpm configuration
#define BPM 120.0

void button_handler(const monome_event_t *e, void *user_data);
mumble_t * mumble_init(mumble_t* mumble);
unsigned char * midi_data_from_monome_event(unsigned char * midi_data, const monome_event_t *e, void *user_data);
void mumble_write_to_midi_device(mumble_t * mumble, unsigned char * midi_data);

#endif
