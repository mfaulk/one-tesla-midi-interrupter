#include "envelope.h"
#include "note.h"
#include "system.h"

float envelope_scaler(volatile note* n) {
  if (n->period > 3500 && n->playtime < 8000) {
    return (float) (n->playtime) / (8000.0f);
  }
  return 1.0f;
}
