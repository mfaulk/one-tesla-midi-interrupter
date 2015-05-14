/*
 * Global variables
 */
 
#ifndef __SHARED_H

extern volatile boolean sustain;
extern volatile note* notes, *note_1, *note_2;
extern volatile int timestamp;
extern volatile long ticks_1, ticks_2;

#define __SHARED_H
#endif
