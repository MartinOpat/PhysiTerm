#pragma once

#include <ncurses.h>

static inline float get_pixel_size_x() { return getmaxx(stdscr); }

static inline float get_pixel_size_y() { return getmaxy(stdscr); }

static inline float get_pixel_radius() {
  return (get_pixel_size_x() + get_pixel_size_y()) / 2.0;
}
