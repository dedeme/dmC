// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader of expresions 'point', 'square - slice' and 'parenthesis'.

#ifndef PT_SQ_PR_READER_H
  #define PT_SQ_PR_READER_H

#include "reader/cdr/cdr.h"
#include "exp.h"
#include "reader/types.h"

/// if they exist. Otherwise returns 'left'
///   tps : List symbols-modules of typed symbols.
///   left: Current expression.
///   cdr : Code reader.
/// Throws EXC_GENERIC
Exp *pt_sq_pr_reader_read (Types *tps, Exp *left, Cdr *cdr);

#endif
