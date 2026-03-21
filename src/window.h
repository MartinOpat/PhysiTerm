#pragma once

#include <ncurses.h>

static inline float get_pixel_size_x() { return (float)COLS / getmaxx(stdscr); }

static inline float get_pixel_size_y() {
  return (float)LINES / getmaxy(stdscr);
}

static inline float get_pixel_aspect() {
  return (float)getmaxx(stdscr) / getmaxy(stdscr) * (float)LINES / COLS;
}
