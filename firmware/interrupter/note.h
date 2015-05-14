#ifndef __NOTE_H

#include "system.h"

typedef struct {
  byte pitch;
  boolean playing;
  boolean sustaining;
  int on_time;
  long sustain_time;
  long period;
  byte scaler;
  int timestamp;
  long playtime;
} note;

void note_kill(volatile note* n);
void make_note(volatile note* n, byte pitch, byte velocity, int timestamp);

#define __NOTE_H
#endif

