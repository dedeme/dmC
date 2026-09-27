// Copyright 16-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Expression writer result.

#ifndef DATA_WR_ERS_H
  #define DATA_WR_ERS_H

#include "data/wrCtx.h"
#include "data/type.h"
#include "data/exp.h"
#include "data/wrSRs.h"

/// Record structure.
struct wrERs_WrERs{
  int is_error;   // TRUE if an error happend.
  Type *tp;       // Expression type.
  char *fun_code; // Code of functions. It can be "".
  char *code;     // C Code.
};

typedef struct wrERs_WrERs WrERs;

/// Error constructor.
///   md : Module.
///   ln : Line number.
///   msg: Error message.
WrERs *wrERs_fail (WrCtx *ctx, int ln, char *msg);

///  Fail of type expected - actual
WrERs *wrERs_failE (WrCtx *ctx, int ln, char *expected, char *actual);

///  Type fail.
WrERs *wrERs_failT (WrCtx *ctx, int ln, Type *t, Exp *e, Type *et);

/// Converts wrSRs.fail in wrERs.fail
WrERs *wrERs_fromWrSRsFail (WrSRs *rs);

/// Result constructor.
///   tp      : Expression type.
///   fun_code: Code of functions.
///   code    : Code body. In a top block, it is placed inside the
///             '_main()' function.
WrERs *wrERs_mk (Type *tp, char *fun_code, char *code);

#endif
