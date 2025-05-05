// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdBytes.h"
#include "comp/built.h"

WrERs *mdBytes_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "add")) return built_mk_rs("[<bytes><bytes>|<bytes>]", md, sym);
  if (!strcmp(sym, "drop")) return built_mk_rs("[<bytes>i|<bytes>]", md, sym);
  if (!strcmp(sym, "fromArr")) return built_mk_rs("[[i]|<bytes>]", md, sym);
  if (!strcmp(sym, "fromStr")) return built_mk_rs("[s|<bytes>]", md, sym);
  if (!strcmp(sym, "get")) return built_mk_rs_ex("[<bytes>i|i]", ctx, ln, md, sym);
  if (!strcmp(sym, "new")) return built_mk_rs("[i|<bytes>]", md, sym);
  if (!strcmp(sym, "set")) return built_mk_rs_ex("[<bytes>ii|]", ctx, ln, md, sym);
  if (!strcmp(sym, "size")) return built_mk_rs("[<bytes>|i]", md, sym);
  if (!strcmp(sym, "take")) return built_mk_rs("[<bytes>i|<bytes>]", md, sym);
  if (!strcmp(sym, "toArr")) return built_mk_rs("[<bytes>|[i]]", md, sym);
  if (!strcmp(sym, "toStr")) return built_mk_rs("[<bytes>|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
