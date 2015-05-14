#include "handlers.h"
#include "timers.h"
#include "constants.h"
#include "shared.h"
#include "system.h"
#include "util.h"
#include "note.h"

/*
 * Handle note-on messages
 * by pushing the note into the buffer
 */
void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  
  ledOff(); //just to be safe
  
  //zero velocity, turn the note off
  if (velocity == 0) {
    HandleNoteOff(channel, pitch, velocity);
    return;
  }
  
  //replace sustaining notes with new versions of themselves
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    if (notes[i].pitch == pitch && notes[i].playing) {
      make_note(&notes[i], pitch, velocity, timestamp);
      playNotes();
      return;
    }
  }
  
  //is there a free spot?
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    if (!notes[i].playing) {
      make_note(&notes[i], pitch, velocity, timestamp);
      playNotes();
      return;
    }
  }

  //replace oldest note
  int t = 0xFFFF;
  int m = 0;
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    if (notes[i].playing && notes[i].timestamp < t) {
      m = i;
      t = notes[i].timestamp;
    }
  }
  make_note(&notes[m], pitch, velocity, timestamp);
  
  //call the magic function to play some notes!
  playNotes();
}

/*
 * Handle note-off messages
 * If sustain is not on, kill the note
 * Otherwise, toggle the flag that puts the timers
 * into sustain mode
 */
void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  if (!sustain) {
    for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
      if (notes[i].pitch == pitch && notes[i].playing) {
        note_kill(&notes[i]); 
        playNotes();
        return;
      }
    }
  } else {
    for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
      if (notes[i].pitch == pitch && notes[i].playing) {
        notes[i].sustaining = true;
        playNotes();
        return;
      }
    }
  }
}

void HandleStop() {
  stopTimer1();
  stopTimer2();
}

void HandleContinue() {
  playNotes();
}

void HandleControlChange(byte channel, byte number, byte value) {
  if (number == 0x78 || number == 0x79 || number == 0x7B || number == 0x7C) {
    HandleStop();
  }
  if (number == 0x40) {
    //sustain off! (and kill all sustaining notes as well)
    if (value < 64) {
      sustain = false;
      for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
        if (notes[i].sustaining) {
          note_kill(&notes[i]);
        }
      }
      playNotes();
    } else {
      sustain = true;
    }
  }
}

void HandlePitchBend(byte channel, int value) {
  float fbend_amount = ((float) value)/((float) (0x2000));
  float bender = 1.0f - 0.1 * fbend_amount;
  long temp = (long) (bender * ticks_1);
  if (temp >= TIMER1_MAX) temp = TIMER1_MAX - 1;
  setTimer1Compare((int) temp);
  temp = (long) (bender * ticks_2);
  if (temp >= TIMER2_MAX) temp = TIMER2_MAX - 1;
  setTimer2Compare((int) temp);
}

void HandleSystemReset() {
  stopTimer1();
  stopTimer2();
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    notes[i].playing = false;
    notes[i].sustaining = false;
  }
  sustain = false;
}
