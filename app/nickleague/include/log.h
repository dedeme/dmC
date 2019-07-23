// Copyright 17-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Json messages

#ifndef LOG_H
  #define LOG_H

#include "dmc/std.h"

/// Returns an error message with next Json structure:
///   {
///     "error": <msg>
///   }
void log_error (char *msg);

/// (For debbuging) Equals to log_error, returning the Json structure.
char *log_error_s (char *msg);

/// Returns an ok message with next Json structure:
///   {
///     "error": "",
///     "value: <value>
///   }
void log_ok (Js *value);

/// (For debbuging) Equals to log_ok, returning the Json structure.
char *log_ok_s (Js *value);

#endif
