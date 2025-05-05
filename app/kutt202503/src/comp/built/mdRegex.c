// Copyright 31-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdRegex.h"
#include "comp/built.h"

WrERs *mdRegex_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "matches")) return built_mk_rs_ex("[ss|[s]]", ctx, ln, md, sym);
  if (!strcmp(sym, "replace")) return built_mk_rs_ex("[sss|s]", ctx, ln, md, sym);
  if (!strcmp(sym, "replacef")) return built_mk_rs_ex("[ss[s|s]|s]", ctx, ln, md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
