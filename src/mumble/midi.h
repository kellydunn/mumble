#include <monome.h>

// TODO Research midi device daemon?
#define MIDI_DEVICE "/dev/midi1"

// MIDI Protocol
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80

// MIDI File processing
#define MIDI_MTHD 0x4D54726B // MIDI Header Magic
#define MIDI_MTRK 0x4D54726B // MIDI Chunk Magic

#define MIDI_HEADER_LENGTH 6
#define MIDI_FORMAT_TYPE 0   // TODO Determine if other types of midi tracks should be configurable
#define MIDI_NUM_TRACKS 1    // TODO Determine if other types of midi tracks should be configurable
#define MIDI_MICROSECOND_PER_MINUTE .0000000166667

typedef struct _midi_time_event_t {
  char * delta_time;
  char * midi_event;
} midi_time_event_t;


typedef struct _midi_header_t {
  char * magic;
  uint32_t length;
  int format_type;
  int num_tracks;
  char * time_delta;
} midi_header_t;

typedef struct _midi_chunk_t {
  char * magic;
  uint32_t length;
  midi_time_event_t * time_events;
} midi_chunk_t;

void play_midi(const monome_event_t * e, void * user_data);
void record_midi(const monome_event_t * e, void * user_data);
void playback_midi(const monome_event_t * e, void * user_data);

unsigned char * midi_data_from_monome_event(unsigned char * midi_data, const monome_event_t *e, void *user_data);
