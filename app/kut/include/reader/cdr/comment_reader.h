// Copyright 08-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Comment reader.

#ifndef COMMENT_READER_H
  #define COMMENT_READER_H

#include "cdr.h"

/// Read a comment token (multiline or monoline) or an operator token ('/')
/// Throws EXC_KUT.
Token *comment_reader_read(Cdr *cdr);

#endif
