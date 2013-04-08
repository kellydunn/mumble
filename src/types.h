#include <monome.h>

#ifndef _MUMBLE_TYPES_H
#define _MUMBLE_TYPES_H

// GLOBALS
// Convience boolean handling
typedef enum { false, true } bool;

// mumble.h
typedef struct mumble mumble_t;

// muxer.h
typedef struct mumble_muxer mumble_muxer_t;
typedef struct mumble_dispatcher mumble_dispatcher_t;

// session.h
typedef struct mumble_session mumble_session_t;

// sample.h
typedef struct mumble_sample mumble_sample_t;

// midi.h
typedef struct midi_time_event midi_time_event_t;
typedef struct midi_header midi_header_t;
typedef struct midi_chunk midi_chunk_t;

// mumble.h [impl]
struct mumble {
  int midi_fd;
  monome_t * monome;
  mumble_muxer_t * muxer;
  mumble_session_t * session;
  int root;
  int velocity;
  float bpm;
};

// muxer.h [impl]
typedef void (*dispatch_func)(const monome_event_t *e, void *user_data);

struct mumble_dispatcher {
  dispatch_func (*dispatch_func)(const monome_event_t *e, void *user_data);
};

struct mumble_muxer {
  mumble_dispatcher_t* dispatchers;
};

// session.h [impl]
struct mumble_session {
  // TODO Implement
  bool recording;
  mumble_sample_t * samples;
};

// sample.h [impl]
struct mumble_sample {
  // TODO Implement
  int inst;
  midi_time_event_t * event;
};

// midi.h [impl]
struct midi_time_event {
  char * delta_time;
  char * midi_event;
};

struct midi_header {
  char * magic;
  uint32_t length;
  int format_type;
  int num_tracks;
  char * time_delta;
};

struct midi_chunk {
  char * magic;
  uint32_t length;
  midi_time_event_t * time_events;
};

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

// TODO Research midi device daemon?
#define MIDI_DEVICE "/dev/midi1"

#endif
