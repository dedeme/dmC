// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdStr.h"
#include "comp/built.h"

WrERs *mdIter_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "all")) return built_mk_rs("[[!A][A|b]|b]", md, sym);
  if (!strcmp(sym, "any")) return built_mk_rs("[[!A][A|b]|b]", md, sym);
  if (!strcmp(sym, "cat")) return built_mk_rs("[[!A][!A]|[!A]]", md, sym);
  if (!strcmp(sym, "count")) return built_mk_rs("[[!A]|i]", md, sym);
  if (!strcmp(sym, "drop")) return built_mk_rs("[[!A]i|[!A]]", md, sym);
  if (!strcmp(sym, "dropWhile")) return built_mk_rs("[[!A][A|b]|[!A]]", md, sym);
  if (!strcmp(sym, "each")) return built_mk_rs("[[!A][A|]|]", md, sym);
  if (!strcmp(sym, "eachIx")) return built_mk_rs("[[!A][Ai|]|]", md, sym);
  if (!strcmp(sym, "empty")) return built_mk_rs("[[A]|[!A]]", md, sym);
  if (!strcmp(sym, "filter")) return built_mk_rs("[[!A][A|b]|[!A]]", md, sym);
  if (!strcmp(sym, "find")) return built_mk_rs("[[!A][A|b]|[A]]", md, sym);
  if (!strcmp(sym, "fromJs")) return built_mk_rs("[s[s|A]|[!A]]", md, sym);
  if (!strcmp(sym, "hasNext")) return built_mk_rs("[[!A]|b]", md, sym);
  if (!strcmp(sym, "index")) return built_mk_rs("[[!A][A|b]|i]", md, sym);
  if (!strcmp(sym, "map")) return built_mk_rs("[[!A][A|B]|[!B]]", md, sym);
  if (!strcmp(sym, "next")) return built_mk_rs("[[!A]|A]", md, sym);
  if (!strcmp(sym, "new")) return built_mk_rs("[[.O[O|[E]]]|[!E]]", md, sym);
  if (!strcmp(sym, "peek")) return built_mk_rs("[[!A]|A]", md, sym);
  if (!strcmp(sym, "reduce")) return built_mk_rs("[[!A]B[BA|B]|B]", md, sym);
  if (!strcmp(sym, "take")) return built_mk_rs("[[!A]i|[!A]]", md, sym);
  if (!strcmp(sym, "takeWhile")) return built_mk_rs("[[!A][A|b]|[!A]]", md, sym);
  if (!strcmp(sym, "toJs")) return built_mk_rs("[[!A][A|s]|s]", md, sym);
  if (!strcmp(sym, "unary")) return built_mk_rs("[A|[!A]]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
