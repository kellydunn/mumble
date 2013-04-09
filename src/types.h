// These are the structs, anonymous functions, and other general types
// That are used by mumble to do all sorts of wonderful things.

#ifndef _MUMBLE_TYPES_H
#define _MUMBLE_TYPES_H

#include <monome.h>
#include "time.h"

// Convience boolean handling
typedef enum { false, true } bool;

// mumble.h
typedef struct mumble mumble_t;

// muxer.h
typedef struct mumble_muxer mumble_muxer_t;
typedef struct mumble_dispatcher mumble_dispatcher_t;

// session.h
typedef struct mumble_session mumble_session_t;

// loop.h
typedef struct mumble_loop mumble_loop_t;

// midi.h
typedef struct mumble_midi_event mumble_midi_event_t;
typedef struct mumble_midi_header mumble_midi_header_t;
typedef struct mumble_midi_chunk mumble_midi_chunk_t;

// list.h
typedef struct mumble_list mumble_list_t;
typedef struct mumble_list_node mumble_list_node_t;

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
  bool recording;
  mumble_loop_t * loops;
};

// loop.h [impl]
struct mumble_loop {
  struct timespec start, stop;
  int inst;
  mumble_list_t * events;
};

// midi.h [impl]
struct mumble_midi_event {
  struct timespec timestamp;
  char * midi_event;
};

struct mumble_midi_header {
  char * magic;
  uint32_t length;
  int format_type;
  int num_tracks;
  char * time_delta;
};

struct mumble_midi_chunk {
  char * magic;
  uint32_t length;
  mumble_midi_event_t * events;
};

// list.h [impl]
struct mumble_list {
  mumble_list_node_t * head;
  mumble_list_node_t * tail;
};

struct mumble_list_node {
  void * data;    // The MIDI Event
  int delay;      // The delay until the next event
  mumble_list_node_t * next;
  mumble_list_node_t * prev;
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
