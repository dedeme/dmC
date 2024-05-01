// Copyright 02-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token reader.

#ifndef READ_TK_READER_H
  #define READ_TK_READER_H

#include "read/token.h"

/// Read one token.
Token *tk_reader_read (char *kt_code, int nline, int nchar);

#endif
