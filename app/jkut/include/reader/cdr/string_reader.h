// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// String reader.

#ifndef STRING_READER_H
  #define STRING_READER_H

#include "cdr.h"

/// Read a string token (with '-', "-" or """-""" ).
/// Throws EXC_KUT.
Token *string_reader_read(Cdr *cdr, char ch, char *blanks);

#endif
