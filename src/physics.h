#pragma once

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

static inline Vecf add(Vecf v1, Vecf v2) {
  return (Vecf){{v1.x + v2.x}, {v1.y + v2.y}};
}

static inline Vecf mul(Vecf v1, float a) {
  v1.x *= a;
  v1.y *= a;
  return v1;
}
