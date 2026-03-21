#pragma once

#include "algorithm.h"
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

void physics_update();
void draw(); // TODO: This function should be in its own file. However, that
             // requires creating an application-wide context variable
