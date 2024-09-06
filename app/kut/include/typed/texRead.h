// Copyright 23-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression reader.

#ifndef TYPED_TEX_READ_H
  #define TYPED_TEX_READ_H

#include "kut/buf.h"
#include "stat.h"
#include "exp.h"
#include "theap.h"

Ttype tex_read (Buf *bf, Theap *th, StatCode *stat, Exp *exp);

#endif
