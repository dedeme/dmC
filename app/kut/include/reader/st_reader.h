// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statements reader.

#ifndef ST_READER_H
  #define ST_READER_H

#include "reader/cdr/cdr.h"
#include "stat.h"
#include "reader/types.h"

/// Read a statement.
StatCode *st_reader_read(Types *tps, Cdr *cdr);

#endif
