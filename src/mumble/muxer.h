#ifndef _MUMBLE_MUXER_H
#define _MUMBLE_MUXER_H

#include "stdlib.h"
#include "stdio.h"
#include <monome.h>
#include "../types.h"

mumble_muxer_t * mumble_muxer_init();

void handle_func(mumble_muxer_t * muxer, int monome_position, dispatch_func func);

#endif

