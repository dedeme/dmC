// Copyright 31-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef IO_H
  #define IO_H

#include "dmc/std.h"

/// Execute 'fn' for each file of 'dir' and its subdirectories.
void io_scan(char *dir, void (*fn)(char *));

#endif
