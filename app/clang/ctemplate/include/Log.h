// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef LOG_H
  #define LOG_H

#include "dmc/std.h"

///
typedef struct log_Log Log;

/// Start with a line number equals 0
Log *log_new(char *file);

/// Increments line number
void log_inc(Log *this);

///
void log_msg(Log *this, char *code, char *msg);

#endif
