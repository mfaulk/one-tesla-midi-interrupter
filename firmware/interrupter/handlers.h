/*
 * MIDI Event Handlers
 * These functions are automatically called by the MIDI library
 * When MIDI events are received
 */
 
#ifndef __HANDLERS_H

#include "system.h"

void HandleNoteOn(byte channel, byte pitch, byte velocity);
void HandleNoteOff(byte channel, byte pitch, byte velocity);
void HandleStop();
void HandleContinue();
void HandleControlChange(byte channel, byte number, byte value);
void HandlePitchBend(byte channel, int value);
void HandleSystemReset();

#define __HANDLERS_H
#endif
