// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expressions reader.

#ifndef EXREADER_H
  #define EXREADER_H

#include "kut/opt.h"
#include "reader/cdr/cdr.h"
#include "reader/types.h"
#include "exp.h"

/// Read the next expression.
/// Throws EXC_KUT
Exp *ex_reader_read (Types *tps, Cdr *cdr);

/// Read the next single expression.
/// Throws EXC_KUT
Exp *ex_reader_read1 (Types *tps, Cdr *cdr);

#endif
