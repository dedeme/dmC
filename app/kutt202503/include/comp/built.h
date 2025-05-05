// Copyright 25-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built functions reader.

#ifndef COMP_BUILT_H
  #define COMP_BUILT_H

#include "data/wrCtx.h"
#include "data/wrERs.h"

/// Returns a function.
///   ctx: Writer context.
///   nl : Statement line.
///   md : Function module.
///   sym: Function identifier.
WrERs *built_get (WrCtx *ctx, int ln, char *md, char *sym);

/// Make normal response.
WrERs *built_mk_rs (char *tp, char *md, char *sym);

/// Make response with execption.
WrERs *built_mk_rs_ex (char *tp, WrCtx *ctx, int ln, char *md, char *sym);

#endif
