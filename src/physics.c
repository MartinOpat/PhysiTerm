#include "physics.h"
#include "algorithm.h"
#include "particles.h"
#include "vec.h"
#include "window.h"
#include <ncurses.h>
#include <stdlib.h>

Objects *o;

// ================ Constants ================
Vecf g = {{0}, {1.0}}; // gravity
float dt = 0.1;

// ================ Creation ================
void add_particle(Particle p) {
  o->ps[o->currSizePs] = p;
  o->currSizePs++;

  if (o->currSizePs == o->maxSizePs) {
    o->maxSizePs *= 2;
    o->ps = realloc(o->ps, o->maxSizePs * sizeof(Particle));
  }
}

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
    Vecf v = get_random_vec(0, 10, 0, 2);
    Particle p = {{{0.0}, {0.0}}, v, 'o'};
    add_particle(p);
  }
}

void destroy_world() {
  free(o->ps);
  free(o);
}

// ================ Actual Physics ================
void update_velocities() {
  for (int i = 0; i < o->currSizePs; ++i) {
    o->ps[i].vel = add(o->ps[i].vel, mul(g, dt));
  }
}

void update_positions() {
  for (int i = 0; i < o->currSizePs; ++i) {
    o->ps[i].pos = add(o->ps[i].pos, mul(o->ps[i].vel, dt));
  }
}

void apply_boundary() {
  for (int i = 0; i < o->currSizePs; ++i) {
    Particle *p = &(o->ps[i]);
    if (p->pos.x <= 0 || p->pos.x >= COLS - 1) {
      // Friction at boundary collision
      p->vel = mul(p->vel, 0.8);

      // Boundary
      p->vel.x = -p->vel.x;
      p->pos.x = max(0.0, min(p->pos.x, (float)(COLS - 1)));
    }
    if (p->pos.y <= 0 || p->pos.y >= LINES - 1) {
      // Friction at boundary collision
      p->vel = mul(p->vel, 0.8);

      // Boundary
      p->vel.y = -p->vel.y;
      p->pos.y = max(0.0, min(p->pos.y, (float)(LINES - 1)));
    }
  }
}

void handle_collision(Particle *p1, Particle *p2) {
  // Helper vars.
  float d = dist(p1->pos, p2->pos);
  Vecf n = div_vec(sub(p2->pos, p1->pos), d);
  float overlap = 2 * get_pixel_radius() - d;

  Vecf v_rel = sub(p2->vel, p1->vel);
  float speed_norm = dot(v_rel, n);
  float imp = -speed_norm;

  // Update positions
  p1->pos = sub(p1->pos, mul(n, overlap / 2.0));
  p2->pos = add(p2->pos, mul(n, overlap / 2.0));

  // Update velocities
  if (speed_norm <= 0) {
    p1->vel = sub(p1->vel, mul(n, imp));
    p2->vel = add(p2->vel, mul(n, imp));
  }
}

void handle_collisions() {
  for (int i = 0; i < o->currSizePs; ++i) {
    for (int j = i + 1; j < o->currSizePs; ++j) {
      Particle p1 = o->ps[i];
      Particle p2 = o->ps[j];
      if ((int)p1.pos.x == (int)p2.pos.x && (int)p1.pos.y == (int)p2.pos.y) {
        printf("Collision!\n");
        handle_collision(&p1, &p2);
      }
    }
  }
}

void physics_update() {
  update_velocities();
  update_positions();
  handle_collisions();
  apply_boundary();
}

// ================ Drawing Utils. ================
void draw() {
  for (int i = 0; i < o->currSizePs; ++i) {
    Particle *p = &(o->ps[i]);
    mvaddch((int)p->pos.y, (int)p->pos.x, p->symbol);
  }
}
