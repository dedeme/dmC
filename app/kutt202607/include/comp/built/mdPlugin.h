// Copyright 18-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Built-in functions.

#ifndef COMP_BUILT_MD_PLUGIN_H
  #define COMP_BUILT_MD_PLUGIN_H

#include "data/wrCtx.h"
#include "data/wrERs.h"

WrERs *mdPlugin_process (WrCtx *ctx, int ln, char *md, char *sym);

#endif
