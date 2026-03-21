#include "physics.h"
#include "vec.h"
#include <stdlib.h>

Objects *o;

// ================ Helpers ================
void add_particle(Particle p) {
  o->ps[o->currSizePs] = p;
  o->currSizePs++;

  if (o->currSizePs == o->maxSizePs) {
    o->maxSizePs *= 2;
    o->ps = realloc(o, o->maxSizePs);
  }
}

// ================ Exported ================

void init_world(int num_init_particles) {
  init_objects();
  init_particles(num_init_particles);
}

void init_objects() {
  o = malloc(sizeof(Objects));

  o->maxSizePs = 64;
  o->currSizePs = 0;
  o->ps = malloc(o->maxSizePs * sizeof(Particle));
}

void init_particles(int num) {
  for (int i = 0; i < num; ++i) {
    Vecf v = get_random_vec(0, 5, 0, 5);
    Particle p = {{{0.0}, {0.0}}, v, 'o'};
    add_particle(p);
  }
}

void destroy_world() {
  free(o->ps);
  free(o);
}
