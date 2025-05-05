// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdB64.h"
#include "comp/built.h"

WrERs *mdB64_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "decode")) return built_mk_rs_ex("[s|s]", ctx, ln, md, sym);
  if (!strcmp(sym, "decodeBytes")) return built_mk_rs_ex("[s|<bytes>]", ctx, ln, md, sym);
  if (!strcmp(sym, "encode")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "encodeBytes")) return built_mk_rs("[<bytes>|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
