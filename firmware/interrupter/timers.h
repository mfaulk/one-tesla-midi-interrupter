/*
 * Backend timers
 * These functions do all the work
 * That produces sparks
 */
 
#ifndef __TIMERS_H

#include "system.h"
#include "constants.h"
#include "note.h"

void startTimer1(byte index);
void startTimer2(byte index);
void stopTimer1();
void stopTimer2();
void ISR_backend(volatile note* n);

#define __TIMERS_H
#endif
