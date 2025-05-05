// Copyright 28-Mar-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "comp/built/mdArr.h"
#include "comp/built.h"

WrERs *mdArr_process (WrCtx *ctx, int ln, char *md, char *sym) {
  if (!strcmp(sym, "all")) return built_mk_rs("[[A][A|b]|b]", md, sym);
  if (!strcmp(sym, "any")) return built_mk_rs("[[A][A|b]|b]", md, sym);
  if (!strcmp(sym, "cat")) return built_mk_rs("[[A][A]|]", md, sym);
  if (!strcmp(sym, "clear")) return built_mk_rs("[[A]|]", md, sym);
  if (!strcmp(sym, "copy")) return built_mk_rs("[[A]|[A]]", md, sym);
  if (!strcmp(sym, "drop")) return built_mk_rs("[[A]i|[A]]", md, sym);
  if (!strcmp(sym, "dropWhile")) return built_mk_rs("[[A][A|b]|[A]]", md, sym);
  if (!strcmp(sym, "duplicates"))
    return built_mk_rs("[[A][AA|b]|[.[A][A]]]", md, sym);
  if (!strcmp(sym, "each")) return built_mk_rs("[[A][A|]|]", md, sym);
  if (!strcmp(sym, "eachIx")) return built_mk_rs("[[A][Ai|]|]", md, sym);
  if (!strcmp(sym, "filter")) return built_mk_rs("[[A][A|b]|[A]]", md, sym);
  if (!strcmp(sym, "filterIn")) return built_mk_rs("[[A][A|b]|]", md, sym);
  if (!strcmp(sym, "find")) return built_mk_rs("[[A][A|b]|[A]]", md, sym);
  if (!strcmp(sym, "fromIter")) return built_mk_rs("[[!A]|[A]]", md, sym);
  if (!strcmp(sym, "fromJs")) return built_mk_rs("[s[s|A]|[A]]", md, sym);
  if (!strcmp(sym, "get")) return built_mk_rs("[[A]i|A]", md, sym);
  if (!strcmp(sym, "index")) return built_mk_rs("[[A][A|b]|i]", md, sym);
  if (!strcmp(sym, "insert")) return built_mk_rs_ex("[[A]iA|]", ctx, ln, md, sym);
  if (!strcmp(sym, "insertArr"))
    return built_mk_rs_ex("[[A]i[A]|]", ctx, ln, md, sym);
  if (!strcmp(sym, "insertRange"))
    return built_mk_rs_ex("[[A]i[A]ii|]", ctx, ln, md, sym);
  if (!strcmp(sym, "join")) return built_mk_rs("[[s]s|s]", md, sym);
  if (!strcmp(sym, "map")) return built_mk_rs("[[A][A|B]|[B]]", md, sym);
  if (!strcmp(sym, "new")) return built_mk_rs("[Ai|[A]]", md, "new_fill");
  if (!strcmp(sym, "peek")) return built_mk_rs_ex("[[A]|A]", ctx, ln, md, sym);
  if (!strcmp(sym, "pop")) return built_mk_rs_ex("[[A]|A]", ctx, ln, md, sym);
  if (!strcmp(sym, "push")) return built_mk_rs("[[A]A|]", md, sym);
  if (!strcmp(sym, "reduce")) return built_mk_rs("[[A]B[BA|B]|B]", md, sym);
  if (!strcmp(sym, "remove")) return built_mk_rs_ex("[[A]i|A]", ctx, ln, md, sym);
  if (!strcmp(sym, "removeRange"))
    return built_mk_rs_ex("[[A]ii|[A]]", ctx, ln, md, sym);
  if (!strcmp(sym, "reverse")) return built_mk_rs("[[A]|[A]]", md, sym);
  if (!strcmp(sym, "reverseIn")) return built_mk_rs("[[A]|]", md, sym);
  if (!strcmp(sym, "set")) return built_mk_rs("[[A]iA|]", md, sym);
  if (!strcmp(sym, "setArr")) return built_mk_rs_ex("[[A]i[A]|]", ctx, ln, md, sym);
  if (!strcmp(sym, "setRange")) return built_mk_rs_ex("[[A]i[A]ii|]", ctx, ln, md, sym);
  if (!strcmp(sym, "shift")) return built_mk_rs_ex("[[A]|A]", ctx, ln, md, sym);
  if (!strcmp(sym, "shuffle")) return built_mk_rs("[[A]|]", md, sym);
  if (!strcmp(sym, "size")) return built_mk_rs("[[A]|i]", md, sym);
  if (!strcmp(sym, "sort")) return built_mk_rs("[[A][AA|b]|]", md, sym);
  if (!strcmp(sym, "take")) return built_mk_rs("[[A]i|[A]]", md, sym);
  if (!strcmp(sym, "takeWhile")) return built_mk_rs("[[A][A|b]|[A]]", md, sym);
  if (!strcmp(sym, "toIter")) return built_mk_rs("[[A]|[!A]]", md, sym);
  if (!strcmp(sym, "toJs")) return built_mk_rs("[[A][A|s]|s]", md, sym);
  if (!strcmp(sym, "unshift")) return built_mk_rs("[[A]A|]", md, sym);
  if (!strcmp(sym, "unzip")) return built_mk_rs("[[[.AB]]|[.[A][B]]]", md, sym);
  if (!strcmp(sym, "unzip3"))
    return built_mk_rs("[[[.ABC]]|[.[A][B][C]]]", md, sym);
  if (!strcmp(sym, "zip")) return built_mk_rs("[[A][B]|[[.AB]]]", md, sym);
  if (!strcmp(sym, "zip3")) return built_mk_rs("[[A][B][C]|[[.ABC]]]", md, sym);

  return wrERs_fail(ctx, ln, str_f("Function '%s.%s' not found.", md, sym));
}
