// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader of expresions 'point', 'square - slice' and 'parenthesis'.

#ifndef PT_SQ_PR_READER_H
  #define PT_SQ_PR_READER_H

#include "reader/cdr/cdr.h"
#include "exp.h"
#include "reader/types.h"

/// Reads anexed expressions 'point', 'square - slice' and 'parenthesis',
/// if they exist. Otherwise returns 'left'
///   tps : List symbols-modules of typed symbols.
///   left: Current expression.
///   cdr : Code reader.
/// Throws EXC_KUT
Exp *pt_sq_pr_reader_read (Types *tps, Exp *left, Cdr *cdr);

#endif
