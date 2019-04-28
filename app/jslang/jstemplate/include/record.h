// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef RECORD_H
  #define RECORD_H

#include "dmc/std.h"

/// Returns error
char *record_init_new (char *line);

/// Returns error
char *record_read_end_new (void);

/// Returns error
char *record_field_new (char *line);

/// Returns code to write
char *record_code_new (void);

/// Clear static variables
void record_free (void);
#endif
