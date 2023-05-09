// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Comment reader.

#ifndef COMMENT_READER_H
  #define COMMENT_READER_H

#include "cdr.h"

/// Read a comment token (multiline or monoline) or an operator token ('/')
/// Throws Exception.
Token *comment_reader_read(Cdr *cdr, char *blanks);

#endif
