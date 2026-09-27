// Copyright 18-Apr-2026 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdPlugin.h"
#include "comp/built.h"

WrERs *mdPlugin_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "close")) return built_mk_rs("[<plugin>|]", md, sym);
  if (!strcmp(sym, "exec"))
    return built_mk_rs_ex("[[R]<plugin>sA|R]", ctx, ln, md, sym);
  if (!strcmp(sym, "open")) return built_mk_rs_ex("[s|<plugin>]", ctx, ln, md, sym);
  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
