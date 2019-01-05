// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File reader with line number.

#ifndef FREADER_H
  #define FREADER_H

#include "dmc/std.h"

///
typedef struct freader_Freader Freader;

///
Freader *freader_new(const char *file);

///
void freader_close(Freader *this);

/// Returns next line or "" if end of file is reached.
char *freader_line_new(Freader *this);

/// Sets a message with file and line number.
void freader_msg(char **final_msg, Freader *this, char *msg);

#endif
