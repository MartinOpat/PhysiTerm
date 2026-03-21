#pragma once

// Logs are stored in `debug.log` file.
// To see logs live, open another terminal and run: `tail -f debug.log`

void init_logging();
void close_logging();

void log_str(const char *s, ...);
