// Copyright 23-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Block statements writer.

#ifndef COMP_BLOCK_WRITER_H
  #define COMP_BLOCK_WRITER_H

#include "kut/arr.h"
#include "data/wrCtx.h"
#include "data/wrSRs.h"

/// Token types
typedef enum {
  blockWriter_top, blockWriter_function, blockWriter_param, blockWriter_inner
} BlockT;

/// Write code and returns its result.
///   rtp     : (Arr<Type>) Expected type or empty Array.
///   generics: (Map<Type>) Known generics.
///   kind    : One of 'top', 'function' or 'inner'.
///   is_loop : 'true' if it is a loop block. (To allow break and continue).
///   is_try  : 'true' if it is a try block.
///   ctx     : Writer context.
///   ln      : Line number.
///   sts     : (Arr<stat>) Block statements.
WrSRs *blockWriter_run(
  Arr *tp, Map *generics, BlockT kind, int is_loop, int is_try,
  WrCtx *ctx, int ln, Arr *sts
);

#endif
