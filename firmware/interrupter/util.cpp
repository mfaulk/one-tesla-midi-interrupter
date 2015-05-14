#include "util.h"
#include "timers.h"
#include "constants.h";
#include "system.h"
#include "shared.h"
#include "note.h"

long getFrequency(byte pitch) {
  return (long) (220.0 * pow(pow(2.0, 1.0/12.0), pitch - 57) + 0.5);
}

int getOnTime(long frequency) {
  int index = (int) floor(frequency / 100.0);
  if (index > ON_TIME_ARRAY_LENGTH - 1) {return 0;}
  else {return LOOKUP_TABLE_SCALE * on_times[index];}
}

void playNotes() {
  stopTimer1();
  stopTimer2();
  int t1 = -1;
  int t2 = -1;
  byte m1 = 0;
  byte m2 = 0;
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    if (notes[i].timestamp > t1 && notes[i].playing) {
      t2 = t1;
      m2 = m1;
      t1 = notes[i].timestamp;
      m1 = i;
      continue;
    }
    if (notes[i].timestamp > t2 && notes[i].playing) {
      t2 = notes[i].timestamp;
      m2 = i;
    }
  }
  if (t1 < 0 && t2 < 0) return;
  if (t1 < 0) {
    startTimer1(m2);
    return;
  }
  if (t2 < 0) {
    startTimer1(m1);
    return;
  }
  if (notes[m2].pitch < TIMER_2_MIN && notes[m1].pitch >= TIMER_2_MIN) {
    int temp = m1;
    m1 = m2;
    m2 = temp;
  }
  if (notes[m2].pitch >= TIMER_2_MIN) {
    startTimer1(m1);
    startTimer2(m2);
    return;
  }
  
  t2 = -1;
  for (int i = 0; i < NOTE_BUFFER_SIZE; i++) {
    if (notes[i].playing && notes[i].pitch >= TIMER_2_MIN && notes[i].timestamp > t2) {
      t2 = notes[i].timestamp;
      m2 = i;
    }
  }
  startTimer1(m1);
  if (t2 < 0) return;
  startTimer2(m2);
}
