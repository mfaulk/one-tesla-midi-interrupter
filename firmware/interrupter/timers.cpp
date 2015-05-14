#include "timers.h"
#include "constants.h"
#include "shared.h"
#include "util.h"
#include "system.h"
#include "note.h"
#include "envelope.h"

void startTimer1(byte index) {
  note_1 = &notes[index];
  note_1->playtime = 0;
  
  long ticks = CPU_CLK * note_1->period / 8;
  
  if (ticks <= TIMER1_MAX) setPrescaler1_8();
  else if ((ticks /= 4) <= TIMER1_MAX) setPrescaler1_16();
  
  ticks--;
  
  ticks_1 = ticks;
  
  setTimer1Compare(ticks);
  if (note_1->on_time != 0) engageISR1();
  
  note_1->playing = true;
}

void startTimer2(byte index) {
  note_2 = &notes[index];
  note_2->playtime = 0;
  
  long ticks = CPU_CLK * note_2->period;
  
  if (ticks <= TIMER2_MAX) setPrescaler2_1();
  else if ((ticks /= 8) <= TIMER2_MAX) setPrescaler2_8();
  else if ((ticks /= 4) <= TIMER2_MAX) setPrescaler2_32();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_64();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_128();
  else if ((ticks /= 2) <= TIMER2_MAX) setPrescaler2_256();
  else if ((ticks /= 4) <= TIMER2_MAX) setPrescaler2_1024();
  
  ticks--;
  
  ticks_2 = ticks;
  
  setTimer2Compare(ticks);
  if (note_2->on_time != 0) engageISR2();
  
  note_2->playing = true;
}

void stopTimer1() {
  disengageISR1();  // Prevent timer1 from calling the ISR
}

void stopTimer2() {
  disengageISR2();  // Prevent timer2 from calling the ISR
}

ISR (TIMER1_COMPA_vect, ISR_BLOCK) {
  ISR_backend(note_1);
  clearCompareFlag2();
}

ISR(TIMER2_COMPA_vect, ISR_BLOCK) {
  ISR_backend(note_2);
  clearCompareFlag1();
}

void ISR_backend(volatile note* n) {
  n->playtime += n->period;
  uint8_t val = ADCH;
  float scaler = (float) (val) / 256.0f;
  int t = (int) (n->on_time * scaler);
  scaler = (float) (n->scaler) / 256.0f;
  t = (int) (t * scaler);
  scaler = envelope_scaler(n);
  t = (int) (t * scaler);
  if (n->sustaining) {
    scaler = map_sustain(n->sustain_time);
    t = (int) (t * scaler);
    n->sustain_time -= n->period;
    if (t < 3) {
      note_kill(n);
      ledOff();
      for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
        if (notes[i].sustaining && &notes[i] != note_1 && &notes[i] != note_2) {
          notes[i].sustain_time -= n->playtime;
        }
      }
      playNotes();
    }
  }
  if (t < MIN_ON_TIME && !n->sustaining) t = MIN_ON_TIME;
  ledOn();
  delayMicroseconds(t);
  ledOff();
  delayMicroseconds(t);
}
