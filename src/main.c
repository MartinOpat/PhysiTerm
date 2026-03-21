#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logs.h"
#include "physics.h"

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
  // printf("\033[?1003h\n"); // Mouse movement too

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

      // case KEY_MOUSE:
      //   MEVENT event;
      //   if (getmouse(&event) == OK) {
      //     printw("MOUSEEEE\n");
      //   }
      //   break;
    }

    // Draw
    draw();
    refresh();
    napms(16);
  }

  // Shutdown
  // printf("\033[?1003l\n"); // Disable mouse movement terminal events
  close_logging();
  destroy_world();
  endwin();
  return EXIT_SUCCESS;
}
