#include <bits/time.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logs.h"
#include "physics.h"
#include "terminal.h"

static struct timespec last_frame;

int main(int argc, char *argv[]) {
  // Handle argvs
  int numParticles = 128; // default
  if (argc > 1) {
    char *p;
    numParticles = (int)strtol(argv[1], &p, 10);
  }

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  // Initialize logging
  init_logging();

  // Enable mouse events
  // mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  init_mouse_tracking();

  // Randomize seed
  srand(time(NULL));

  // Create world
  init_world(numParticles);

  // Main loop
  int quit = 0;
  int dtsPerFrame = 1;
  while (!quit) {
    clear();

    // Update  physics
    for (int i = 0; i < dtsPerFrame; ++i) {
      physics_update();
    }

    int ch = getch();
    switch (ch) {
    case 'q':
      quit = 1;
      break;

    case KEY_RESIZE:
      resizeterm(0, 0);
      wake_all();
      break;
    }

    // Mouse events handled separately
    int mx, my;
    if (read_mouse(&mx, &my))
      update_mouse(mx, my);

    // Draw
    draw();
    refresh();

    // Sleep  until 16ms (60fps)
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long elapsed_ms = (now.tv_sec - last_frame.tv_sec) * 1000 +
                      (now.tv_nsec - last_frame.tv_nsec) / 1000000;
    long rem = 16 - elapsed_ms;
    if (rem > 0) {
      napms(rem);
    }
    clock_gettime(CLOCK_MONOTONIC, &last_frame);
  }

  // Shutdown
  close_mouse_tracking();
  close_logging();
  destroy_world();
  endwin();
  return EXIT_SUCCESS;
}
