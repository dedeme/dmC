// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdStr.h"
#include "comp/built.h"

WrERs *mdStr_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "cmp")) return built_mk_rs("[ss|i]", md, sym);
  if (!strcmp(sym, "ends")) return built_mk_rs("[ss|b]", md, sym);
  if (!strcmp(sym, "fmt"))
      return wrERs_fail(ctx, ln, str_f(
        "Function '%s.%s' only can be used in function call.",
        md, sym
      ));
  if (!strcmp(sym, "fromIso")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "fromRunes")) return built_mk_rs("[[s]|s]", md, sym);
  if (!strcmp(sym, "fromUtf16")) return built_mk_rs("[[i]|s]", md, sym);
  if (!strcmp(sym, "index")) return built_mk_rs("[ss|i]", md, sym);
  if (!strcmp(sym, "indexFrom")) return built_mk_rs("[ssi|i]", md, sym);
  if (!strcmp(sym, "lastIndex")) return built_mk_rs("[ss|i]", md, sym);
  if (!strcmp(sym, "left")) return built_mk_rs("[si|s]", md, sym);
  if (!strcmp(sym, "len")) return built_mk_rs("[s|i]", md, sym);
  if (!strcmp(sym, "ltrim")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "replace")) return built_mk_rs("[sss|s]", md, sym);
  if (!strcmp(sym, "right")) return built_mk_rs("[si|s]", md, sym);
  if (!strcmp(sym, "rtrim")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "split")) return built_mk_rs("[ss|[s]]", md, sym);
  if (!strcmp(sym, "splitTrim")) return built_mk_rs("[ss|[s]]", md, sym);
  if (!strcmp(sym, "starts")) return built_mk_rs("[ss|b]", md, sym);
  if (!strcmp(sym, "sub")) return built_mk_rs("[sii|s]", md, sym);
  if (!strcmp(sym, "toLower")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "toRunes")) return built_mk_rs("[s|[s]]", md, sym);
  if (!strcmp(sym, "toUpper")) return built_mk_rs("[s|s]", md, sym);
  if (!strcmp(sym, "toUtf16")) return built_mk_rs("[s|[i]]", md, sym);
  if (!strcmp(sym, "trim")) return built_mk_rs("[s|s]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
