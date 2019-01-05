// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reads template code

#ifndef READER_H
  #define READER_H

#include "dmc/std.h"
#include "Freader.h"

///
void reader_run(
  char **error_null,
  // Arr[Def]
  Arr *defs,
  Freader *slck,
  FileLck *tlck
);

#endif
