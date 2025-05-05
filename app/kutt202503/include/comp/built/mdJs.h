// Copyright 31-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in functions.

#ifndef COMP_BUILT_MD_JS_H
  #define COMP_BUILT_MD_JS_H

#include "data/wrCtx.h"
#include "data/wrERs.h"

WrERs *mdJs_process (WrCtx *ctx, int ln, char *md, char *sym);

#endif
