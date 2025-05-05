// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement writer result.

#ifndef DATA_WR_SRS_H
  #define DATA_WR_SRS_H

#include "data/wrCtx.h"
#include "data/type.h"
#include "data/exp.h"

typedef struct wrERs_WrERs WrERs;

/// WrSRs field 'stopped' values.
typedef enum {
  wrSRs_no,       // Statement does not stop code.
  wrSRs_by_break, // Statement stop code with break or continue.
  wrSRs_by_return // Statement stop code with return.
} WrSRsStopT;

/// Record structure.
typedef struct {
  int is_error;      // TRUE if an error happend.
  WrSRsStopT stopped; // One of 'wrSRs_no', 'wrSRs_by_break' or 'wrSRs_by_return'.
  char *code;        // C Code.
} WrSRs;

/// Error constructor.
///   md : Module.
///   ln : Line number.
///   msg: Error message.
WrSRs *wrSRs_fail (WrCtx *ctx, int ln, char *msg);

///  Fail of type expected - actual
WrSRs *wrSRs_failE (WrCtx *ctx, int ln, char *expected, char *actual);

///  Type fail.
/// \<wrCtx>, i, <type>, <exp>, <type> -> <wrERs>
WrSRs *wrSRs_failT (WrCtx *ctx, int ln, Type *t, Exp *e, Type *et);

/// Converts wrERs.fail in wrSRs.fail
WrSRs *wrSRs_fromWrERsFail (WrERs *rs);

/// Result constructor.
///   stopped: One of 'no', 'byBreak' or 'byReturn'.
///   code   : C Code.
WrSRs *wrSRs_mk (WrSRsStopT stopped, char *code);

#endif
