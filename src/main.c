#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "algorithm.h"
#include "particles.h"
#include "physics.h"

int main(int argc, char *argv[]) {

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);

  // Randomize seed
  srand(time(NULL));

  // Test
  Particle p = {40, 10, 5.0, 0.0, 'o'};
  Vecf g = {0, 1.0}; // gravity

  int quit = 0;

  int dts_per_frame = 1;
  float dt = 0.1;
  while (!quit) {
    clear();

    // Update  physics
    for (int i = 0; i < dts_per_frame; ++i) {
      p.vel.x += g.x * dt;
      p.vel.y += g.y * dt;

      p.pos.x += p.vel.x * dt;
      p.pos.y += p.vel.y * dt;
    }
    printw("(x,y) = %f %f\n", p.pos.x, p.pos.y);
    printw("(vx,vy) = %f %f\n", p.vel.x, p.vel.y);

    // Border
    if (p.pos.x <= 0 || p.pos.x >= COLS - 1) {
      p.vel.x = -p.vel.x * 0.8;
      p.pos.x = max(0.0, min(p.pos.x, (float)(COLS - 1)));
    }
    if (p.pos.y <= 0 || p.pos.y >= LINES - 1) {
      p.vel.y = -p.vel.y * 0.8;
      p.pos.y = max(0.0, min(p.pos.y, (float)(LINES - 1)));
    }

    // Draw
    mvaddch((int)p.pos.y, (int)p.pos.x, p.symbol);
    refresh();
    napms(16);

    int ch = getch();
    switch (ch) {
    case 'q':
      quit = 1;
      break;
    }
  }

  // Shutdown
  endwin();
  return EXIT_SUCCESS;
}
