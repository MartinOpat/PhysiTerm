#pragma once

// Mouse
void init_mouse_tracking();
void close_mouse_tracking();

int read_mouse(int *x, int *y);

// UI
extern volatile int quit_requested;
