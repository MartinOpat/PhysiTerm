#include "terminal.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// ==================== Mouse ====================

static volatile int mouse_x = 0, mouse_y = 0;
static volatile int mouse_updated = 0;
static pthread_t mouse_thread;
static volatile int running = 1;
static FILE *tty;

// TODO: Rename function, decouple logic for 'q' and mouse
static void *mouse_reader(void *arg) {
  int fd = fileno(tty);
  char buf[64];

  while (running) {
    char c;
    if (read(fd, &c, 1) != 1)
      continue;

    // 'q'
    if (c == 'q') {
      quit_requested = 1;
      continue;
    }

    // mouse
    if (c != '\033')
      continue;
    if (read(fd, &c, 1) != 1 || c != '[')
      continue;
    if (read(fd, &c, 1) != 1 || c != '<')
      continue;

    // Read the rest into buffer until we hit M or m
    int i = 0;
    while (i < (int)sizeof(buf) - 1) {
      if (read(fd, &buf[i], 1) != 1)
        break;
      if (buf[i] == 'M' || buf[i] == 'm') {
        buf[i + 1] = '\0';
        break;
      }
      i++;
    }

    int btn, x, y;
    if (sscanf(buf, "%d;%d;%d", &btn, &x, &y) == 3) {
      mouse_x = x - 1;
      mouse_y = y - 1;
      mouse_updated = 1;
    }
  }
  return NULL;
}

void init_mouse_tracking() {
  tty = fopen("/dev/tty", "r+");
  fprintf(tty, "\033[?1003h\033[?1006h");
  fflush(tty);
  pthread_create(&mouse_thread, NULL, mouse_reader, NULL);
}

void close_mouse_tracking() {
  running = 0;
  fprintf(tty, "\033[?1003l\033[?1006l");
  fflush(tty);
  pthread_cancel(mouse_thread);
  pthread_join(mouse_thread, NULL);
  fclose(tty);
}

int read_mouse(int *x, int *y) {
  if (!mouse_updated)
    return 0;
  *x = mouse_x;
  *y = mouse_y;
  mouse_updated = 0;
  return 1;
}

// ==================== I/O ====================
volatile int quit_requested = 0;
