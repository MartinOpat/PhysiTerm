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
