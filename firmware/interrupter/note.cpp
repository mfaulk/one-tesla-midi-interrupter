/*
 * note related functions
 * we use a struct and not a class
 * to make porting to C-only platforms easier
 */
 
#include "note.h"
#include "system.h"
#include "util.h"

void note_kill(volatile note* n) {
  n->playing = false;
  n->sustaining = false;
}

void make_note(volatile note* n, byte pitch, byte velocity, int timestamp) {
  n->pitch = pitch;
  long freq = getFrequency(pitch);
  n->period = 1000000 / freq;
  n->on_time = getOnTime(freq);
  n->scaler = map_velocity(velocity);
  n->playing = true;
  n->sustaining = false;
  n->sustain_time = SUSTAIN_TIME;
  n->timestamp = timestamp;
  n->playtime = 0;
}
