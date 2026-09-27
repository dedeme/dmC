// Copyright 31-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdJs.h"
#include "comp/built.h"

WrERs *mdJs_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "isNull")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "ra")) return built_mk_rs("[s|[s]]", md, sym);
  if (!strcmp(sym, "rb")) return built_mk_rs("[s|b]", md, sym);
  if (!strcmp(sym, "rf")) return built_mk_rs("[s|f]", md, sym);
  if (!strcmp(sym, "ri")) return built_mk_rs("[s|i]", md, sym);
  if (!strcmp(sym, "ro")) return built_mk_rs("[s|{s}]", md, sym);
  if (!strcmp(sym, "rs")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "wa")) return built_mk_rs("[[s]|s]", md, sym);
  if (!strcmp(sym, "wb")) return built_mk_rs("[b|s]", md, sym);
  if (!strcmp(sym, "wf")) return built_mk_rs("[f|s]", md, sym);
  if (!strcmp(sym, "wf2")) return built_mk_rs("[fi|s]", md, sym);
  if (!strcmp(sym, "wi")) return built_mk_rs("[i|s]", md, sym);
  if (!strcmp(sym, "wn")) return built_mk_rs("[|s]", md, sym);
  if (!strcmp(sym, "wo")) return built_mk_rs("[{s}|s]", md, sym);
  if (!strcmp(sym, "ws")) return built_mk_rs("[s|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
