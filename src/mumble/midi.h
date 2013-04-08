#ifndef _MUMBLE_MIDI_H
#define _MUMBLE_MIDI_H

#include <monome.h>
#include "../types.h"

void play_midi(const monome_event_t * e, void * user_data);
void record_midi(const monome_event_t * e, void * user_data);

#endif
