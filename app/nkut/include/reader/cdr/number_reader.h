// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Number reader.

#ifndef NUMBER_READER_H
  #define NUMBER_READER_H

#include "cdr.h"

/// Read a number token (Int or Float).
/// Throws Exception
Token *number_reader_read(Cdr *cdr, char ch, char *blanks);

#endif
