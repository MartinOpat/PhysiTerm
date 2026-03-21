#pragma once

#include "vec.h"

#define SLEEP_SPEED_TOLERANCE 0.05f
#define FRAMES_TO_SLEEP 15

typedef struct {
  Vecf pos;
  Vecf vel;

  char symbol;

  int idleFrames;
  int isSleeping;
} Particle;
