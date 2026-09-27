// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Number reader.

#ifndef NUMBER_READER_H
  #define NUMBER_READER_H

#include "cdr.h"

/// Read a number token (Int or Float).
/// Throws EXC_KUT
Token *number_reader_read(Cdr *cdr, char ch);

#endif
