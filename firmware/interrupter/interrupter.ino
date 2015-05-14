/*
 * Polyphonic MIDI-based DRSSTC interrupter (Version 1.6.5)
 * Copyright 2013 oneTesla, LLC
 * All work licensed under the GNU GPL, version 3.0
 */
 
#include "MIDI.h"
#include "util.h"
#include "timers.h"
#include "handlers.h"
#include "constants.h"
#include "system.h"
#include "note.h"

//this line is a workaround to make the code compile in Arduino 1.0.3
//we don't actually use SPI
#include <SPI.h>

volatile boolean sustain;
volatile note* notes;
volatile note* note_1, *note_2;
volatile long ticks_1, ticks_2;
volatile int timestamp = 0;

void fixedLoop(void);

void setup() {
  //allocate the note buffer
  notes = (note*) malloc(NOTE_BUFFER_SIZE * sizeof(note));
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) notes[i].playing = false;
  note_1 = &notes[0];
  note_2 = &notes[1];
  
  //turn off the transmitter, just in case
  ledOff();
  
  // Set up the ADC registers, timers, and pin modes
  setupADC();
  setupTimers();
  setupPins();
  
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(1);    
    
  // Setup MIDI callback functions
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleContinue);
  MIDI.setHandleControlChange(HandleControlChange);
  MIDI.setHandlePitchBend(HandlePitchBend);
  MIDI.setHandleSystemReset(HandleSystemReset);
  
  // Read the position of the mode switch, and enter the correct mode
  if (!MIDI_MODE) fixedLoop();
}

// --- MIDI mode and fixed mode loops --- //

//MIDI loop
void loop() {
  MIDI.read();
}

//Fixed-frequency (standalone) loop
void fixedLoop() {
  while (1) {
    byte v = readFreqPot();    
    int period_delay = 3500 + (long) 1500 * v / 256;
    
    v = readPWPot();    
    float on_time_scale = (float) v / 256;
    int on_time = getOnTime(1 / period_delay);
    on_time *= on_time_scale;
    if(on_time < MIN_ON_TIME) {on_time = MIN_ON_TIME;}
    
    period_delay -= 600;
    
    ledOn();
    delayMicroseconds(on_time);
    ledOff();
    delayMicroseconds(period_delay);
  }
}
