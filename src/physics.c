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
    Particle p = {{{0.0}, {0.0}}, v, 'o', 0, 0};
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
    if (o->ps[i].isSleeping)
      continue;

    // Integrate
    o->ps[i].vel = add(o->ps[i].vel, mul(g, dt));

    // Cap
    o->ps[i].vel.x = max(-MAX_SPEED, min(MAX_SPEED, o->ps[i].vel.x));
    o->ps[i].vel.y = max(-MAX_SPEED, min(MAX_SPEED, o->ps[i].vel.y));
  }
}

void update_positions() {
  for (int i = 0; i < o->currSizePs; ++i) {
    if (o->ps[i].isSleeping)
      continue;
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
  float imp = -0.75 * speed_norm; // Restitution e = 0.5: coef = -(1+e)/2
  imp = max(-MAX_IMPULSE, min(MAX_IMPULSE, imp)); // cap impulse forces

  // Update sleeping
  if (speed_norm < -WAKE_THRESHOLD) {
    p1->isSleeping = 0;
    p1->idleFrames = 0;
    p2->isSleeping = 0;
    p2->idleFrames = 0;
  } else if (p1->isSleeping && p2->isSleeping) {
    return;
  } else if (!p1->isSleeping && p2->isSleeping) {
    p1->idleFrames += 2; // Fall asleep faster
  } else if (p1->isSleeping && !p2->isSleeping) {
    p2->idleFrames += 2; // Fall asleep faster
  }

  // Only collide if particles are moving towards each other
  if (speed_norm <= 0) {
    // Update positions
    p1->pos = sub(p1->pos, mul(n, overlap / 2.0));
    p2->pos = add(p2->pos, mul(n, overlap / 2.0));

    // Update velocities
    p1->vel = sub(p1->vel, mul(n, imp));
    p2->vel = add(p2->vel, mul(n, imp));
  }
}

void handle_collisions() {
  for (int i = 0; i < o->currSizePs; ++i) {
    for (int j = i + 1; j < o->currSizePs; ++j) {
      Particle *p1 = &(o->ps[i]);
      Particle *p2 = &(o->ps[j]);

      // The check distane shuold be at least 2x the pixel radius since that is
      // what gets enforced in the collision handling
      if (fabsf(p1->pos.x - p2->pos.x) <= 2.1 * get_pixel_radius() &&
          fabsf(p1->pos.y - p2->pos.y) <= 2.1 * get_pixel_radius()) {
        handle_collision(p1, p2);
      }
    }
  }
}

void update_sleeping() {
  for (int i = 0; i < o->currSizePs; ++i) {
    Particle *p = &o->ps[i];
    if (p->vel.x < SLEEP_SPEED_TOLERANCE && p->vel.y < SLEEP_SPEED_TOLERANCE) {
      p->idleFrames++;
      if (p->idleFrames >= FRAMES_TO_SLEEP) {
        p->isSleeping = 1;
        p->vel.x = 0.0f;
        p->vel.y = 0.0f;
      }
    }
  }
}

void physics_update() {
  update_velocities();
  update_positions();
  handle_collisions();
  apply_boundary();
  update_sleeping();
}

// ================ Drawing Utils. ================
void draw() {
  for (int i = 0; i < o->currSizePs; ++i) {
    Particle *p = &(o->ps[i]);
    mvaddch((int)p->pos.y, (int)p->pos.x, p->symbol);
  }
}
