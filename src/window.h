#pragma once

#include <ncurses.h>

static inline float get_pixel_size_x() {
  return (float)LINES / getmaxx(stdscr);
}

static inline float get_pixel_size_y() { return (float)COLS / getmaxy(stdscr); }

static inline float get_pixel_radius() {
  return (get_pixel_size_x() + get_pixel_size_y()) / 2.0;
}
