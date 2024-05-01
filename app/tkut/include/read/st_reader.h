// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement reader.

#ifndef READ_ST_READER_H
  #define READ_ST_READER_H

#include "run/rn_code.h"

/// Read one statement.
/// This function modifies rn_code and returns the rest of kt_code that
/// remains without reading.
/// When all text is read, return "".
char *st_reader_read (char *kt_code, RnCode *rn_code);

#endif
