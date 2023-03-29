// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Reader of expresions 'point', 'square - slice' and 'parenthesis'.

#ifndef PT_SQ_PR_READER_H
  #define PT_SQ_PR_READER_H

#include "reader/cdr/cdr.h"
#include "exp.h"

/// Reads anexed expressions 'point', 'square - slice' and 'parenthesis',
/// if they exist. Otherwise returns 'left'
/// Throws EXC_KUT
Exp *pt_sq_pr_reader_read (Exp *left, Cdr *cdr);

#endif
