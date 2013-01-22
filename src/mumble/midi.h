#include <monome.h>

// TODO Research midi device daemon?
#define MIDI_DEVICE "/dev/midi"

// MIDI Protocol
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

// ROADMAP Somehow account for custom scales
#define LOW_C 36

// TODO Handle note velocity more elegantly
#define VELOCITY 127 

void play_midi(const monome_event_t * e, void * user_data);
