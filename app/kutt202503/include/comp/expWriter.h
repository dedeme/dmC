// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression writer.

#ifndef COMP_EXP_WRITER_H
  #define COMP_EXP_WRITER_H

#include "data/wrCtx.h"
#include "data/wrERs.h"

/// Write code and returns its result.
WrERs *expWriter_run(WrCtx *ctx, Exp *ex);

/// Process a exp.parenthesis
WrERs *expWriter_parenthesis (WrCtx *ctx, Exp *ex, int is_stat);

#endif
