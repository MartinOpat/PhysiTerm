#pragma once

#include <ncurses.h>

static inline float get_pixel_size_x() { return getmaxx(stdscr) / LINES; }

static inline float get_pixel_size_y() { return getmaxy(stdscr) / COLS; }

static inline float get_pixel_radius() {
  return (get_pixel_size_x() + get_pixel_size_y()) / 2.0;
}
