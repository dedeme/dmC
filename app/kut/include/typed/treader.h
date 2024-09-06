// Copyright 22-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed code reader

#ifndef TYPED_TREADER_H
  #define TYPED_TREADER_H

#include "kut/buf.h"
#include "typed/theap.h"

/// Reads 'st' and writes C code in 'bf'.
/// 'tret' is the expected return type.
void treader_read(Buf *bf, Ttype tret, Theap *th, StatCode *st);

#endif
