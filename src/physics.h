#pragma once

#include "particles.h"
#include "vec.h"

typedef struct {
  Particle *ps;
  int maxSizePs;
  int currSizePs;
} Objects;

void init_world(int num_init_particles);
void destroy_world();

void init_objects();
void init_particles(int num);
