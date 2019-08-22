// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Template type record.

#ifndef RECORD_H
  #define RECORD_H

#include "dmc/std.h"

/// Returns error
char *record_init (char *line);

/// Returns error
char *record_read_end (void);

/// Returns error
char *record_field (char *line);

/// Returns code to write
char *record_code (void);

#endif
