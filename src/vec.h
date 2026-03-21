#pragma once

#include <math.h>
#include <stdlib.h>

#include "window.h"

// Vector
typedef struct {
  union {
    int x;
    int v1;
  };
  union {
    int y;
    int v2;
  };
} Veci;

typedef struct {
  union {
    float x;
    float v1;
  };
  union {
    float y;
    float v2;
  };
} Vecf;

// Basic operations
static inline Vecf add(Vecf v1, Vecf v2) {
  return (Vecf){{v1.x + v2.x}, {v1.y + v2.y}};
}

static inline Vecf sub(Vecf v1, Vecf v2) {
  return (Vecf){{v1.x - v2.x}, {v1.y - v2.y}};
}

static inline Vecf mul(Vecf v1, float a) {
  v1.x *= a;
  v1.y *= a;
  return v1;
}

static inline Vecf div_vec(Vecf v1, float a) {
  v1.x /= a;
  v1.y /= a;
  return v1;
}

static inline float dot(Vecf v1, Vecf v2) { return v1.x * v2.x + v1.y * v2.y; }

// Advanced operations

static inline Vecf get_random_vec(float minx, float maxx, float miny,
                                  float maxy) {
  float x = minx + (maxx - minx) * rand() / RAND_MAX;
  float y = miny + (maxy - miny) * rand() / RAND_MAX;
  return (Vecf){{x}, {y}};
}

static inline float dist(Vecf v1, Vecf v2) {
  return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}
