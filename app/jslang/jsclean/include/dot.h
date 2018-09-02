// Copyright 01-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DOT_H
  #define DOT_H

#include "dmc/std.h"
#include "tp/P.h"

/// Creates and cleans .jsclean_dots.db
void dot_init (void);

/// Returns the list of dot properties of 'file'
Achar *dot_read (P *file);

/// Writes 'list' in 'file'
void dot_write (P *file, Achar *list);

/// Extracts dot properties of 'line', adds them to 'new_dot' and prints
/// message if 'old_dot' does not contain any property.
void dot_print_and_update(
  P *file,
  Achar *old_dot,
  Achar *new_dot,
  char *line,
  int nline
);

#endif
