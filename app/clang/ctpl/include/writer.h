// Copyright 24-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Writes template codes

#ifndef WRITER_H
  #define WRITER_H

#include "dmc/std.h"

/// 'defs' is Arr[Def]
void writer_c(Arr *defs, FileLck *lck);

/// 'defs' is Arr[Def]
void writer_h(Arr *defs, FileLck *lck);

#endif
