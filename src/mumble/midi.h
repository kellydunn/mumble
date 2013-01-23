#include <monome.h>

// TODO Research midi device daemon?
#define MIDI_DEVICE "/dev/snd/midiC1D0"

// MIDI Protocol
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

void play_midi(const monome_event_t * e, void * user_data);
